#pragma once

#include <cstddef>

#include "libipc/export.h"

namespace ipc {
namespace shm {

using id_t = void*;

enum : unsigned {
    create = 0x01,
    open   = 0x02
};

IPC_EXPORT id_t   acquire(char const * name, std::size_t size, unsigned mode = create | open);
IPC_EXPORT void * get_mem(id_t id, std::size_t * size);
IPC_EXPORT void   release(id_t id);
IPC_EXPORT void   remove (id_t id);
IPC_EXPORT void   remove (char const * name);

class IPC_EXPORT handle {
public:
    handle();
    handle(char const * name, std::size_t size, unsigned mode = create | open);
    handle(handle&& rhs);

    ~handle();

    void swap(handle& rhs);
    handle& operator=(handle rhs);

    bool         valid() const;
    std::size_t  size () const;
    char const * name () const;

    bool acquire(char const * name, std::size_t size, unsigned mode = create | open);
    void release();

    void* get() const;

    void attach(id_t);
    id_t detach();

private:
    class handle_;
    handle_* p_;
};

} // namespace shm
} // namespace ipc
