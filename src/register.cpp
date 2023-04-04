#include "drvwrapper.h"
#include "register.h"
#include <sstream>
#include <iostream>
#include  <iomanip>
#include <boost/format.hpp>

const int indent_width = 2;
static std::string indent(int level)
{
    return std::string(level * indent_width, ' ');
}

void Register::write(u_int32_t val)
{
    if (!m_name.empty()) {
        m_drv.log() << "REG: " << boost::format("0x%08x ") % m_offset << " [\""<< m_name <<"\"] " << " <- " << boost::format("0x%08x ") % val;
    } else {
        m_drv.log() << "REG: " << boost::format("0x%08x ") % m_offset << " <- " << boost::format("0x%08x ") % val;
    }
    if (!m_description.empty())
        m_drv.log() << "\t\t" << m_description << std::endl;
    m_drv.write(m_offset, val);
}

uint32_t Register::read()
{
    u_int32_t val;
    val = m_drv.read(m_offset);
    m_drv.log() << __repr__() <<  std::endl;
    return val;
}

std::string Register::__repr__()
{
    std::stringstream ss;
    u_int32_t val;
    val = m_drv.read(m_offset);
    if (!m_name.empty()) {
        ss << "REG: " << boost::format("0x%08x ") % m_offset << " [\""<< m_name <<"\"] " << " : " << boost::format("0x%08x ") % val;
    } else {
        ss << "REG: " << boost::format("0x%08x ") % m_offset << " : " << boost::format("0x%08x ") % val;
     }
    if (!m_description.empty())
        ss << "\t\t" << m_description;
    return ss.str();
}

std::string Register::__str__()
{
    std::ostringstream ss;
    ss << boost::format("0x%08x ") % m_drv.read(m_offset);
    return ss.str();
}

void Register::dump(std::ostream &out)
{
    u_int32_t val;
    val = m_drv.read(m_offset);
    if (!m_name.empty()) {
        out << "REG: " << boost::format("0x%08x ") % m_offset << " [\""<< m_name <<"\"] " << " : " << boost::format("0x%08x ") % val;
    } else {
        out << "REG: " << boost::format("0x%08x ") % m_offset << " : " << boost::format("0x%08x ") % val;
     }
}


Register& RegisterSet::define(uint32_t offset, std::string name, std::string description, bool atomic)
{
    Register &reg = m_drv.m_regs.define(offset, name, description, atomic);
    m_map[offset] = &reg;
    if (!name.empty())
        m_smap[name] = &reg;
    else if (!reg.m_name.empty())
        m_smap[reg.m_name] = &reg;
    return reg;
}

IOMemory &RegisterSet::define_mem(uint32_t offset, uint32_t sz, std::string name, std::string description)
{
    IOMemory &reg = m_drv.m_regs.define_mem(offset, sz, name, description);
    m_map[offset] = &reg;
    if (!name.empty())
        m_smap[name] = &reg;
    else if (!reg.m_name.empty())
        m_smap[reg.m_name] = &reg;
    return reg;
}

RegisterFactory::RegisterFactory(const RegisterFactory &src):RegisterSet(src)
{
    m_is_reg_owner = false;
}

RegisterFactory::RegisterFactory(RegisterFactory &&src):RegisterSet(src)
{
    m_is_reg_owner = src.m_is_reg_owner;
    src.m_is_reg_owner = false;
}

RegisterFactory::~RegisterFactory()
{
    if (!m_is_reg_owner)
        return;
    for (auto iter: m_map){
        auto reg = iter.second;
        delete reg;
    }
}

Register& RegisterFactory::define(uint32_t offset, std::string name, std::string description, bool atomic)
{
    Register *reg;
    if (!name.empty())
        name = handle_name_colision(name);

    auto iter = m_map.find(offset);
    if ( iter == m_map.end()) {
        if (atomic)
            reg = new AtomicRegister(m_drv, offset, name, description);
           else
            reg = new Register(m_drv, offset, name, description);
        m_map[offset] = reg;
        if (!name.empty())
             m_smap[name] = m_map[offset];
    } else {
        reg = iter->second;
        if (!name.empty()) {
            reg->m_name = name;
            m_smap[name] = reg;
        }
        if (!description.empty()) {
            reg->m_description = description;
        }
    }

    return *reg;
}

IOMemory &RegisterFactory::define_mem(uint32_t offset, uint32_t sz, std::string name, std::string description)
{
    IOMemory *reg;
    if (!name.empty())
        name = handle_name_colision(name);

    auto iter = m_map.find(offset);
    if ( iter == m_map.end()) {
        reg = new IOMemory(m_drv, offset, sz, name, description);
        m_map[offset] = reg;
        if (!name.empty())
             m_smap[name] = m_map[offset];
    } else {
        reg = dynamic_cast<IOMemory*>(iter->second);
        if (!name.empty()) {
            reg->m_name = name;
            m_smap[name] = reg;
        }
        if (!description.empty()) {
            reg->m_description = description;
        }
    }

    return *reg;
}

std::string RegisterFactory::handle_name_colision(const std::string &name)
{
    if (m_smap.find(name) != m_smap.end()) {
        std::string new_name;
        int i = 1;
        do {
            new_name = name + "." + std::to_string(i);
            i++;
        }
        while (m_smap.find(new_name) != m_smap.end());
        std::cerr << "WARNING: Name colision on \""<<name <<"\". Renaming to \""<< new_name << "\"" << std::endl;
        return new_name;
    }
    return std::string(name);
}

std::string RegisterSet::dump(int level)
{
    std::stringstream ss;
    for (auto iter: m_map){
        auto reg = iter.second;
        ss << indent(level);
        reg->dump(ss);
        ss << std::endl;
    }
    return ss.str();
}

std::string LogicBlock::dump(int level)
{
    std::ostringstream ss;
    ss << indent(level);
    ss << "--- BLOCK : " << m_name << " ---" << std::endl;
    std::string s = ss.str();
    s += m_regs.dump(level + 1);
    for (auto c : m_children)
        s += c->dump(level + 1);
    return s;
}

std::string LogicBlock::__repr__()
{
    std::ostringstream ss;
    ss << "--- BLOCK : " << m_name << " ---" << std::endl;
    for (auto r : m_regs.m_map)
        ss << r.second->__repr__() << std::endl;
    for (auto c : m_children)
        ss << c->__repr__() << std::endl;
    return ss.str();
}

void LogicBlock::init()
{
    for (auto &&c: m_children)
        c->init();
}

std::string IOMemoryCell::__repr__()
{
    std::stringstream ss;
    u_int32_t val;
    val = m_drv.read(m_offset);
    ss << "MEMCELL: " << boost::format("0x%08x ") % m_offset << " [\"" << m_mem.m_name << "[sz : " << boost::format("0x%08x ") % (m_offset - m_mem.m_offset) << "] \"] " << " : 0x" << std::hex << val;
    return ss.str();
}

IOMemoryCell::IOMemoryCell(IOMemory &mem, uint32_t offset):
    Register(mem.m_drv, offset, std::string(),std::string()),
    m_mem(mem)
{

}

void IOMemory::dump(std::ostream &out)
{
    uint32_t i;

    if (!m_sz)
        return;

    out << "IOMEM: " << boost::format("0x%08x ") % m_offset  << "\" " << m_name << "\"";
    for (i = 0; i < m_sz / 4; i++) {
        uint32_t val = m_drv.read(m_offset + i * 4);
        if ((i & 0x7) == 0)
            out << std::endl << indent(1) << boost::format("0x%08x : ") % (m_offset + i * 4);
        out << boost::format("%08x ") % val;
    }
}


std::string IOMemory::__repr__()
{
    std::stringstream ss;
    dump(ss);
    return ss.str();
}

std::string IOMemory::__str__()
{
    std::ostringstream out;
    uint32_t i;
    for (i = 0; i < m_sz / 4; i++) {
        uint32_t val = m_drv.read(m_offset + i * 4);
        out << boost::format("%08x ") % val;
        if ((i & 0x7) == 0X7)
            out << std::endl;
    }
    return out.str();
}


IOMemoryCell IOMemory::operator[](uint32_t offset)
{
    return IOMemoryCell(*this, m_offset + offset * 4);
}
