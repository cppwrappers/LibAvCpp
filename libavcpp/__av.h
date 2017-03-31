#ifndef __AV_H
#define __AV_H

#include <mutex>

namespace av {
class __av {
public:
    __av();
private:
    std::once_flag _register_flag;
};
}//namespace av
#endif // __AV_H
