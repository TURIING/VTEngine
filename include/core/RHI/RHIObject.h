//
// Created by turiing on 2025/1/9.
//

#ifndef RHIOBJECT_H
#define RHIOBJECT_H

template<class T>
class RHIObject {
public:
    virtual ~RHIObject() = default;
    [[nodiscard]] T GetHandle() const { return m_pHandle; }

protected:
    T m_pHandle = nullptr;
};
#endif //RHIOBJECT_H
