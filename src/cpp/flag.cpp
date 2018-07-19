boolValue *newBoolValue(bool val, bool *p)
{
    *p = val;
    return (*boolValue)(p);
}
moku::error boolValue::Set(std::string s)
{
    moku::error err{};
    bool v{false};

    std::tie(v, err) = strconv::ParseBool(s);
    *this = boolValue(v);
    return err;
}
moku::interface boolValue::Get()
{
    return bool(*this);
}
std::string boolValue::String()
{
    return strconv::FormatBool(bool(*this));
}
bool boolValue::IsBoolFlag()
{
    return true;
}
intValue *newIntValue(int val, int *p)
{
    *p = val;
    return (*intValue)(p);
}
moku::error intValue::Set(std::string s)
{
    moku::error err{};
    int64_t v{0};

    std::tie(v, err) = strconv::ParseInt(s, 0, strconv::IntSize);
    *this = intValue(v);
    return err;
}
moku::interface intValue::Get()
{
    return int(*this);
}
std::string intValue::String()
{
    return strconv::Itoa(int(*this));
}
int64Value *newInt64Value(int64_t val, int64_t *p)
{
    *p = val;
    return (*int64Value)(p);
}
moku::error int64Value::Set(std::string s)
{
    moku::error err{};
    int64_t v{0};

    std::tie(v, err) = strconv::ParseInt(s, 0, 64);
    *this = int64Value(v);
    return err;
}
moku::interface int64Value::Get()
{
    return int64_t(*this);
}
std::string int64Value::String()
{
    return strconv::FormatInt(int64_t(*this), 10);
}
uintValue *newUintValue(unsigned int val, unsigned int *p)
{
    *p = val;
    return (*uintValue)(p);
}
moku::error uintValue::Set(std::string s)
{
    moku::error err{};
    uint64_t v{0};

    std::tie(v, err) = strconv::ParseUint(s, 0, strconv::IntSize);
    *this = uintValue(v);
    return err;
}
moku::interface uintValue::Get()
{
    return unsigned int(*this);
}
std::string uintValue::String()
{
    return strconv::FormatUint(uint64_t(*this), 10);
}
uint64Value *newUint64Value(uint64_t val, uint64_t *p)
{
    *p = val;
    return (*uint64Value)(p);
}
moku::error uint64Value::Set(std::string s)
{
    moku::error err{};
    uint64_t v{0};

    std::tie(v, err) = strconv::ParseUint(s, 0, 64);
    *this = uint64Value(v);
    return err;
}
moku::interface uint64Value::Get()
{
    return uint64_t(*this);
}
std::string uint64Value::String()
{
    return strconv::FormatUint(uint64_t(*this), 10);
}
stringValue *newStringValue(std::string val, std::string *p)
{
    *p = val;
    return (*stringValue)(p);
}
moku::error stringValue::Set(std::string val)
{
    *this = stringValue(val);
    return nil;
}
moku::interface stringValue::Get()
{
    return std::string(*this);
}
std::string stringValue::String()
{
    return std::string(*this);
}
float64Value *newFloat64Value(double val, double *p)
{
    *p = val;
    return (*float64Value)(p);
}
moku::error float64Value::Set(std::string s)
{
    moku::error err{};
    double v{0};

    std::tie(v, err) = strconv::ParseFloat(s, 64);
    *this = float64Value(v);
    return err;
}
moku::interface float64Value::Get()
{
    return double(*this);
}
std::string float64Value::String()
{
    return strconv::FormatFloat(double(*this), 'g', -1, 64);
}
durationValue *newDurationValue(Duration val, Duration *p)
{
    *p = val;
    return (*durationValue)(p);
}
moku::error durationValue::Set(std::string s)
{
    moku::error err{};
    Duration v{};

    std::tie(v, err) = time::ParseDuration(s);
    *this = durationValue(v);
    return err;
}
moku::interface durationValue::Get()
{
    return time::Duration(*this);
}
std::string durationValue::String()
{
    return (*time::Duration)(this)->String();
}
moku::slice<Flag *> sortFlags(std::map<std::string, Flag *> flags)
{
    int i{0};
    StringSlice list{};
    moku::slice<Flag *> result{};

    list = make(sort::StringSlice, len(flags));
    i = 0;
    {
        Flag *f{std::nullptr};
        for (f : moku::range_value<std::map<std::string, Flag *>>(flags)) {
            list[i] = f->Name;
            i++;
        }
    }
    list.Sort();
    result = make(moku::slice<*Flag>, len(list));
    {
        int i{0};
        std::string name{""};
        for (std::tie(i, name) : moku::range_key_value<StringSlice>(list)) {
            result[i] = flags[name];
        }
    }
    return result;
}
Writer FlagSet::out()
{
    if (moku::is_nil(this->output)) {
        return os::Stderr;
    }
    return this->output;
}
void FlagSet::SetOutput(Writer output)
{
    this->output = output;
}
void FlagSet::VisitAll(std::function<void(Flag *)> fn)
{
    {
        Flag *flag{std::nullptr};
        for (flag :
             moku::range_value<moku::slice<Flag *>>(sortFlags(this->formal))) {
            fn(flag);
        }
    }
}
void VisitAll(std::function<void(Flag *)> fn)
{
    CommandLine->VisitAll(fn);
}
void FlagSet::Visit(std::function<void(Flag *)> fn)
{
    {
        Flag *flag{std::nullptr};
        for (flag :
             moku::range_value<moku::slice<Flag *>>(sortFlags(this->actual))) {
            fn(flag);
        }
    }
}
void Visit(std::function<void(Flag *)> fn)
{
    CommandLine->Visit(fn);
}
Flag *FlagSet::Lookup(std::string name)
{
    return this->formal[name];
}
Flag *Lookup(std::string name)
{
    return CommandLine->formal[name];
}
moku::error FlagSet::Set(std::string name, std::string value)
{
    moku::error err{};
    Flag *flag{std::nullptr};
    bool ok{false};

    std::tie(flag, ok) = this->formal[name];
    if (!ok) {
        return fmt::Errorf("no such flag -%v", name);
    }
    err = flag::Value.Set(value);
    if (!moku::is_nil(err)) {
        return err;
    }
    if (moku::is_nil(this->actual)) {
        this->actual = make(std::map<std::string, *Flag>);
    }
    this->actual[name] = flag;
    return nil;
}
moku::error Set(std::string name, std::string value)
{
    return CommandLine->Set(name, value);
}
bool isZeroValue(Flag *flag, std::string value)
{
    Type typ{};
    Value z{};

    typ = reflect::TypeOf(moku::make_iface<Value>(&{flag Value}));
    if (typ.Kind() == reflect::Ptr) {
        z = reflect::New(typ.Elem());
    } else {
        z = reflect::Zero(typ);
    }
    if (value == moku::type_assert<Value>(z.Interface()).String()) {
        return true;
    }
    if ((value == "false")) {
    _ident_0_:
        return true;
    } else if ((value == "")) {
    _ident_1_:
        return true;
    } else if ((value == "0")) {
    _ident_2_:
        return true;
    }
    return false;
}
std::tuple<std::string, std::string> UnquoteUsage(Flag *flag)
{
}
