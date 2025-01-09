//
// Created by turiing on 2025/1/9.
//

#ifndef RHIBUILDERBASE_H
#define RHIBUILDERBASE_H

#include "common/common.h"

template <class T>
class RHIBuilderBase<T> {
public:
    virtual ~RHIBuilderBase() = default;
    virtual std::shared_ptr<T> Build() = 0;
};
#endif //RHIBUILDERBASE_H
