Element *Element::Next()
{
    {
        Element *p{std::nullptr};

        p = this->next;

        if (!moku::is_nil(this->list) && p != &this->list->root) {
            return p;
        }
    }
    return nil;
}
Element *Element::Prev()
{
    {
        Element *p{std::nullptr};

        p = this->prev;

        if (!moku::is_nil(this->list) && p != &this->list->root) {
            return p;
        }
    }
    return nil;
}
List *List::Init()
{
    this->root.next = &this->root;
    this->root.prev = &this->root;
    this->len = 0;
    return this;
}
List *New()
{
    return new (List)->Init();
}
int List::Len()
{
    return this->len;
}
Element *List::Front()
{
    if (this->len == 0) {
        return nil;
    }
    return this->root.next;
}
Element *List::Back()
{
    if (this->len == 0) {
        return nil;
    }
    return this->root.prev;
}
void List::lazyInit()
{
    if (moku::is_nil(this->root.next)) {
        this->Init();
    }
}
Element *List::insert(Element *e, Element *at)
{
    Element *n{std::nullptr};

    n = at->next;
    at->next = e;
    e->prev = at;
    e->next = n;
    n->prev = e;
    e->list = this;
    this->len++;
    return e;
}
Element *List::insertValue(moku::interface v, Element *at)
{
    return this->insert(&Element{{Value, v}}, at);
}
Element *List::remove(Element *e)
{
    e->prev->next = e->next;
    e->next->prev = e->prev;
    e->next = nil;
    e->prev = nil;
    e->list = nil;
    this->len--;
    return e;
}
moku::interface List::Remove(Element *e)
{
    if (e->list == this) {
        this->remove(e);
    }
    return e->Value;
}
Element *List::PushFront(moku::interface v)
{
    this->lazyInit();
    return this->insertValue(moku::make_iface<moku::interface>(v), &this->root);
}
Element *List::PushBack(moku::interface v)
{
    this->lazyInit();
    return this->insertValue(moku::make_iface<moku::interface>(v),
                             this->root.prev);
}
Element *List::InsertBefore(moku::interface v, Element *mark)
{
    if (mark->list != this) {
        return nil;
    }
    return this->insertValue(moku::make_iface<moku::interface>(v), mark->prev);
}
Element *List::InsertAfter(moku::interface v, Element *mark)
{
    if (mark->list != this) {
        return nil;
    }
    return this->insertValue(moku::make_iface<moku::interface>(v), mark);
}
void List::MoveToFront(Element *e)
{
    if (e->list != this || this->root.next == e) {
        return;
    }
    this->insert(this->remove(e), &this->root);
}
void List::MoveToBack(Element *e)
{
    if (e->list != this || this->root.prev == e) {
        return;
    }
    this->insert(this->remove(e), this->root.prev);
}
void List::MoveBefore(Element *e, Element *mark)
{
    if (e->list != this || e == mark || mark->list != this) {
        return;
    }
    this->insert(this->remove(e), mark->prev);
}
void List::MoveAfter(Element *e, Element *mark)
{
    if (e->list != this || e == mark || mark->list != this) {
        return;
    }
    this->insert(this->remove(e), mark);
}
void List::PushBackList(List *other)
{
    this->lazyInit();
    {
        Element *e{std::nullptr};
        int i{0};
        for (std::tie(i, e) =
                     std::tuple<int, Element *>(other->Len(), other->Front());
             i > 0;
             std::tie(i, e) = std::tuple<int, Element *>(i - 1, e->Next())) {
            this->insertValue(moku::make_iface<moku::interface>(&{e Value}),
                              this->root.prev);
        }
    }
}
void List::PushFrontList(List *other)
{
    this->lazyInit();
    {
        Element *e{std::nullptr};
        int i{0};
        for (std::tie(i, e) =
                     std::tuple<int, Element *>(other->Len(), other->Back());
             i > 0;
             std::tie(i, e) = std::tuple<int, Element *>(i - 1, e->Prev())) {
            this->insertValue(moku::make_iface<moku::interface>(&{e Value}),
                              &this->root);
        }
    }
}
int main()
{
    _main();
    return 0;
}
