#pragma once

namespace AlgGeom
{
class InterfaceAdapter
{
public:
    virtual void renderGuiObject() = 0;
    virtual ~InterfaceAdapter()    = default;
};
}    // namespace AlgGeom
