////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* UICollection<T>::Get(uint32_t index) const
{
    return (T*)BaseCollection::GetComponent(index).GetPtr();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void UICollection<T>::Set(uint32_t index, T* item)
{
    BaseCollection::Set(index, item);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
int UICollection<T>::Add(T* item)
{
    return BaseCollection::Add(item);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void UICollection<T>::Insert(uint32_t index, T* item)
{
    BaseCollection::Insert(index, item);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool UICollection<T>::Contains(T* item) const
{
    return BaseCollection::IndexOfComponent(item) >= 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
int UICollection<T>::IndexOf(T* item) const
{
    return BaseCollection::IndexOfComponent(item);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool UICollection<T>::Remove(T* item)
{
    return BaseCollection::Remove(item);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
const TypeClass* UICollection<T>::GetItemType() const
{
    return TypeOf<T>();
}

}
