#pragma once

namespace GDSA::Render
{
class InterfaceAdapter
{
public:
    virtual void renderGuiObject() = 0;
    virtual ~InterfaceAdapter()    = default;
};
}    // namespace GDSA::Render
