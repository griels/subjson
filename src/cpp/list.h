#pragma once
namespace list {

struct Element {
    Element *next{std::nullptr};
    Element *prev{std::nullptr};
    List *list{std::nullptr};
    moku::interface Value;
    Element *Next();
    Element *Prev();
};
template <>
inline bool moku::is_nil<Element>(const Element &element)
{
    return moku::is_nil<Element *>(element.next) &&
           moku::is_nil<Element *>(element.prev) &&
           moku::is_nil<List *>(element.list) &&
           moku::is_nil<moku::interface>(element.Value);
}
struct List {
    Element root;
    int len{0};
    List *Init();
    int Len();
    Element *Front();
    Element *Back();
    void lazyInit();
    Element *insert(Element *e, Element *at);
    Element *insertValue(moku::interface v, Element *at);
    Element *remove(Element *e);
    moku::interface Remove(Element *e);
    Element *PushFront(moku::interface v);
    Element *PushBack(moku::interface v);
    Element *InsertBefore(moku::interface v, Element *mark);
    Element *InsertAfter(moku::interface v, Element *mark);
    void MoveToFront(Element *e);
    void MoveToBack(Element *e);
    void MoveBefore(Element *e, Element *mark);
    void MoveAfter(Element *e, Element *mark);
    void PushBackList(List *other);
    void PushFrontList(List *other);
};
template <>
inline bool moku::is_nil<List>(const List &list)
{
    return moku::is_nil<Element>(list.root) && moku::is_nil<int>(list.len);
}
List *New();
} // namespace list
