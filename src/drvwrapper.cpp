#include <drvwrapper.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <ios>
#include <linux/ioctl.h>

#define MAGIC_NUMBER            0x91
#define IOCTL_POTDR_INFO        _IOR(MAGIC_NUMBER, 0, struct potdr_info_param)
#define IOCTL_POTDR_ALLOC_MEM	_IOWR(MAGIC_NUMBER, 1, struct potdr_alloc_free_mem_param)
#define IOCTL_POTDR_FREE_MEM	_IOW(MAGIC_NUMBER, 2, struct potdr_alloc_free_mem_param)
#define IOCTL_POTDR_SYNC_DMA	_IOW(MAGIC_NUMBER, 3, struct potdr_dma_param)
#define IOCTL_POTDR_WAIT_EVENT	_IOWR(MAGIC_NUMBER, 5, struct potdr_wait_event_param)

struct potdr_info_param {
    uint32_t reg_area_size;
    uint16_t vendor;
    uint16_t device;
    uint16_t subsystem_vendor;
    uint16_t subsystem_device;
    uint16_t slice;
    uint16_t side;
} __attribute__((aligned(4)));

struct potdr_dma_param {
    uint32_t	phys_src;
    void		*dst;
    uint32_t	sz;
    uint32_t	timeout_ms;
} __attribute__((aligned(4)));

struct potdr_wait_event_param {
    bool        reset_event;
    uint32_t	mask;
    uint32_t	timeout_ms;
} __attribute__((aligned(4)));

struct potdr_alloc_free_mem_param {
    uint32_t	phys_addr;
    uint32_t	sz;
} __attribute__((aligned(4)));


DrvWrapper::DrvWrapper(const std::string &devname):
    m_devname(devname), m_fd(-1),
    m_log(new std::ostringstream()), m_logging_enabled(false),
    m_regs(*this)
{    
    open();
}

DrvWrapper::~DrvWrapper()
{
    close();
    delete m_log;
}

void DrvWrapper::open()
{
    if (m_fd >= 0)
        return;

    int fd = ::open(m_devname.c_str(), O_RDWR);
    if (fd < 0)
        throw std::ios_base::failure(strerror(errno));
    m_fd = fd;

    struct potdr_info_param infos;
    int rc = ::ioctl(fd, IOCTL_POTDR_INFO, &infos);
    if (rc < 0)
        throw std::ios_base::failure(strerror(errno));

    m_reg_area_size = infos.reg_area_size;
    m_device_ids.vendor = infos.vendor;
    m_device_ids.device = infos.device;
    m_device_ids.subsystem_vendor = infos.subsystem_vendor;
    m_device_ids.subsystem_device = infos.subsystem_device;
    m_module_location.slice = infos.slice;
    m_module_location.side = infos.side ? OPS : PWS;

    m_reg_ptrs = (uint32_t*) ::mmap(NULL, m_reg_area_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (m_reg_ptrs == MAP_FAILED) {
        ::close(fd);
        throw std::ios_base::failure(strerror(errno));
    }
}

void DrvWrapper::close()
{
    int fd = m_fd;

    if (m_reg_ptrs) {
        void *p = m_reg_ptrs;
        m_reg_ptrs = NULL;
        ::munmap(p, m_reg_area_size);
    }

    m_fd = -1;
    if (fd >= 0)
        ::close(fd);
}

void DrvWrapper::write(unsigned long offset, uint32_t value)
{
    validate_file_descriptor();
    validate_register_access(offset);
    m_reg_ptrs[offset / 4] = value;
}

uint32_t DrvWrapper::read(unsigned long offset) const
{
    validate_file_descriptor();
    validate_register_access(offset);
    return m_reg_ptrs[offset / 4];
}

void DrvWrapper::validate_file_descriptor() const
{
    if (m_fd < 0)
        throw std::ios_base::failure("Device file not opened");
}

void DrvWrapper::validate_register_access(unsigned long offset) const
{
    if (offset >= m_reg_area_size) {
        // Throw an exception if the offset is out of bounds
        throw std::out_of_range("Register offset out of bounds");
    }

    if (offset & 0x3) {
        throw std::invalid_argument("Invalid offset (must be 32bits aligned)");
    }
}

uint32_t DrvWrapper::wait_event(uint32_t mask, bool reset_event, int timeout_ms)
{
    validate_file_descriptor();

    struct potdr_wait_event_param param = {
        .reset_event = reset_event,
        .mask = (uint32_t) mask,
        .timeout_ms = (uint32_t) timeout_ms
    };
    int rc = ::ioctl(m_fd, IOCTL_POTDR_WAIT_EVENT, &param);
    if (!rc) {
        log() << "EVENT: " << "received event " << std::hex << param.mask << std::endl;
        return param.mask;
    }
    if (errno != ETIMEDOUT) {
        throw std::ios_base::failure(strerror(errno));
    }
    log() << "EVENT: " << "timed out waiting for event " << std::hex << param.mask << std::endl;
    return 0;
}

void DrvWrapper::sync_dma(u_int32_t start, void *dst, uint32_t sz, uint32_t timeout_ms)
{
    validate_file_descriptor();

    struct potdr_dma_param param = {
        .phys_src = start,
        .dst = dst,
        .sz = sz,
        .timeout_ms = timeout_ms
    };

    log() << "DMA: " << "Started DMA from " << std::hex << start << " with size " << sz << std::endl;
    int rc = ::ioctl(m_fd, IOCTL_POTDR_SYNC_DMA, &param);
    if (rc)
        throw std::ios_base::failure(strerror(errno));
    log() << "DMA: " << "Done" << std::endl;
}

uint32_t DrvWrapper::allocate_device_mem(uint32_t sz)
{
    validate_file_descriptor();

    struct potdr_alloc_free_mem_param param = {
      .phys_addr = 0,
      .sz = sz
    };
    int rc = ::ioctl(m_fd, IOCTL_POTDR_ALLOC_MEM, &param);
    if (rc)
        throw std::ios_base::failure(strerror(errno));

    log() << "DMA: " << "Allocated " << sz << "bytes at address " << std::hex << param.phys_addr << std::endl;
    return param.phys_addr;
}

void DrvWrapper::free_device_mem(uint32_t addr, uint32_t sz)
{
    validate_file_descriptor();

    struct potdr_alloc_free_mem_param param = {
      .phys_addr = addr,
      .sz = sz
    };
    int rc = ::ioctl(m_fd, IOCTL_POTDR_FREE_MEM, &param);
    if (rc)
        throw std::ios_base::failure(strerror(errno));
    log() << "DMA: " << "Free " << sz << "bytes at address " << std::hex << addr << std::endl;
}
