std::tuple<moku::interface, moku::error, bool> Group::Do(
        std::string key, std::function<moku::interface, moku::error()> fn)
{
    call *c{std::nullptr};
    moku::error err{};
    bool shared{false};
    moku::interface v{};

    this->mu.Lock();
    if (moku::is_nil(this->m)) {
        this->m = make(std::map<std::string, *call>);
    }
    {
        call *c{std::nullptr};
        bool ok{false};

        std::tie(c, ok) = this->m[key];

        if (ok) {
            c->dups++;
            this->mu.Unlock();
            c->wg.Wait();
            return {c->val, c->err, true};
        }
    }
    c = new (call);
    c->wg.Add(1);
    this->m[key] = c;
    this->mu.Unlock();
    this->doCall(c, key, fn);
    return {c->val, c->err, c->dups > 0};
}
moku::channel<Result, false, true> Group::DoChan(
        std::string key, std::function<moku::interface, moku::error()> fn)
{
}
