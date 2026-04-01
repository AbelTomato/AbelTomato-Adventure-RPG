#pragma once
//可交互的物件继承本接口
class Interactable {
public:
    //默认空实现，重写时返回true就是可交互的物品
    virtual bool CanInteract() {return true;};
    //写入交互逻辑。
    virtual void Interact(){};
    virtual ~Interactable() = default;
};