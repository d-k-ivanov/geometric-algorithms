#pragma once

namespace GDSA
{
class InterfaceAdapter
{
public:
    virtual void renderGuiObject() = 0;
    virtual ~InterfaceAdapter()    = default;
};
}    // namespace GDSA
