#ifndef REGISTER_H
#define REGISTER_H

#include <mutex>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <stdint.h>

#define BIT(x) (1<<x)

class DrvWrapper;
class RegisterSet;
class AtomicRegister;
class IOMemoryCell;
class IOMemory;
class LogicBlock;
class RegisterFactory;

class Register
{
    friend IOMemoryCell;
    friend IOMemory;
    friend AtomicRegister;
    friend RegisterFactory;
    friend RegisterSet;
public:
    void write(u_int32_t val);
    uint32_t read();

    virtual std::string __repr__();
    virtual std::string __str__();
    virtual void dump(std::ostream &out);

    virtual operator uint32_t() { return this->read();}
    virtual Register& operator|=(uint32_t val) { val = this->read() | val; this->write(val); return *this;}
    virtual Register& operator&=(uint32_t val) { val = this->read() & val; this->write(val); return *this;}
    virtual Register& operator^=(uint32_t val) { val = this->read() ^ val; this->write(val); return *this;}
    virtual Register& operator=(uint32_t val) {this->write(val); return *this;}
    //virtual Register& operator=(Register& other) { uint32_t val = other.read(); this->write(val); return *this;}
    virtual ~Register() {};
    uint32_t offset() {return m_offset;};
private:
    Register(DrvWrapper &drv, uint32_t offset, std::string name, std::string description):
        m_drv(drv), m_offset(offset),m_name(name), m_description(description) {};

    DrvWrapper &m_drv;
    uint32_t m_offset;
    std::string m_name;
    std::string m_description;
};


class AtomicRegister: public Register
{
    friend RegisterFactory;
    friend RegisterSet;

public:
    virtual Register& operator|=(uint32_t val) { this->lock(); val = this->read() | val; this->write(val); this->unlock(); return *this;}
    virtual Register& operator&=(uint32_t val) { this->lock(); val = this->read() & val; this->write(val); this->unlock(); return *this;}
    virtual Register& operator^=(uint32_t val) { this->lock(); val = this->read() ^ val; this->write(val); this->unlock(); return *this;}

    void lock() {m_mutex.lock();}
    void unlock() {m_mutex.unlock();}
    virtual ~AtomicRegister() {};

private:
    AtomicRegister(DrvWrapper &drv, uint32_t offset, std::string name, std::string description): Register(drv, offset, name, description) {};
    std::mutex m_mutex;
};


//TODO RegisterArray ou RegisterMem(pour memoire DATA_PTS_MEM)

class RegisterSet
{
    friend LogicBlock;
    friend RegisterFactory;
public:
    RegisterSet(DrvWrapper &drv):m_drv(drv) {}
    Register &define(uint32_t offset, std::string name = "", std::string description = "", bool atomic = true);
    IOMemory &define_mem(uint32_t offset, uint32_t sz, std::string name = "", std::string description = "");
    Register& operator[](uint32_t offset) {return *m_map[offset];}
    Register& operator[](std::string name) {return *m_smap[name];}
    std::string dump(int level = 0);
    Register& get_by_name(std::string &name) {return *m_smap[name];}
    Register& get_by_offset(uint32_t offset) {return *m_map[offset];}
private:
    std::map<uint32_t, Register*> m_map;
    std::map<std::string, Register*> m_smap;
    DrvWrapper &m_drv;
};

class RegisterFactory: public RegisterSet
{
public:
    RegisterFactory(DrvWrapper &drv):RegisterSet(drv) {m_is_reg_owner = true;}
    RegisterFactory(RegisterFactory &&src);
    RegisterFactory(const RegisterFactory &src);
    ~RegisterFactory();
    Register &define(uint32_t offset, std::string name = "", std::string description = "", bool atomic = true);
    IOMemory &define_mem(uint32_t offset, uint32_t sz, std::string name = "", std::string description = "");
private:
    std::string handle_name_colision(const std::string &name);
    bool m_is_reg_owner;
};

#define REGDEF(O,N,D) m_regs.define(offset + O, N, D)
#define MEMDEF(O, SZ, N,D) m_regs.define_mem(offset + O, SZ, N, D)
class LogicBlock
{
public:
    LogicBlock(std::string name, uint32_t offset, LogicBlock *parent, int idx = 0):
        m_name(name), m_offset(offset), m_parent(parent), m_drv(parent->drv()),
        m_regs(parent->drv()), m_idx(idx) {parent->m_children.push_back(this);};

    LogicBlock(std::string name, uint32_t offset, DrvWrapper &drv, int idx = 0):
        m_name(name), m_offset(offset), m_parent(NULL), m_drv(drv),
        m_regs(drv), m_idx(idx) {};
    DrvWrapper& drv() { return m_drv;}
    std::string dump(int level = 0);
    std::string __repr__();

    LogicBlock* root_block() { return m_parent ? m_parent->root_block() : this; }

    /*
     * Basic initialization of the logic block.
     * This function can be used to set-uo the registers with
     * decent default values.
     * This functions must be recursive, all the child loigic blocks
     * must be set up as well.
     */
    virtual void init();

public:
    std::string m_name;
    uint32_t m_offset;
    LogicBlock *m_parent;
    DrvWrapper &m_drv;
    RegisterSet m_regs;
    int m_idx;
private:
    std::vector<LogicBlock*> m_children;
};


class IOMemoryCell: public Register
{
public:
    std::string __repr__();
    IOMemoryCell(IOMemory& mem, uint32_t offset);
    virtual Register& operator=(uint32_t val) {this->write(val); return *this;}
private:
    IOMemory &m_mem;
};

class IOMemory: public Register // IO memory is just one real big register
{
    friend RegisterSet;
    friend RegisterFactory;

public:
    void write(uint32_t offset, u_int32_t val);
    uint32_t read(uint32_t offset);

    std::string __repr__();
    std::string __str__();
    void dump(std::ostream &out);
    IOMemoryCell operator[](uint32_t offset);
private:
    IOMemory(DrvWrapper &drv, uint32_t offset, uint32_t sz, std::string name, std::string description): Register(drv, offset, name, description), m_sz(sz){};
    uint32_t m_sz;
};

#ifdef PYTHON_BINDINGS
#define def_reference(cls,n,r)  def_property_readonly(n, [](cls &m) { return  m.r;}, py::return_value_policy::reference)
#define def_register(cls,n,r)  def_property(n, [](cls &m) { return  m.r.read();}, [](cls &m, uint32_t val) { m.r.write(val);})
#endif
#endif
