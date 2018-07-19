bool deepValueEqual(Value v1,
                    Value v2,
                    std::map<visit, bool> visited,
                    int depth)
{
    std::function<bool(Kind)> hard{std::nullptr};

    if (!v1.IsValid() || !v2.IsValid()) {
        return v1.IsValid() == v2.IsValid();
    }
    if (v1.Type() != v2.Type()) {
        return false;
    }
    hard = [=](Kind k) mutable -> bool {
        if ((k == Map) || (k == Slice) || (k == Ptr) || (k == Interface)) {
        _ident_0_:
            return true;
        }
        return false;
    };
    if (v1.CanAddr() && v2.CanAddr() && hard(v1.Kind())) {
        addr1 = unsafe::Pointer(v1.UnsafeAddr());
        addr2 = unsafe::Pointer(v2.UnsafeAddr());
        if (uintptr_t(addr1) > uintptr_t(addr2)) {
            std::tie(addr1, addr2) = std::tuple<void *, void *>(addr2, addr1);
        }
        typ = v1.Type();
        v = visit{addr1, addr2, typ};
        if (visited[v]) {
            return true;
        }
        visited[v] = true;
    }
    if ((v1.Kind() == Array)) {
    _ident_1_ : {
        int i{0};
        for (i = 0; i < v1.Len(); i++) {
            if (!deepValueEqual(v1.Index(i), v2.Index(i), visited, depth + 1)) {
                return false;
            }
        }
    }
        return true;
    } else if ((v1.Kind() == Slice)) {
    _ident_2_:
        if (v1.IsNil() != v2.IsNil()) {
            return false;
        }
        if (v1.Len() != v2.Len()) {
            return false;
        }
        if (v1.Pointer() == v2.Pointer()) {
            return true;
        }
        {
            int i{0};
            for (i = 0; i < v1.Len(); i++) {
                if (!deepValueEqual(
                            v1.Index(i), v2.Index(i), visited, depth + 1)) {
                    return false;
                }
            }
        }
        return true;
    } else if ((v1.Kind() == Interface)) {
    _ident_3_:
        if (v1.IsNil() || v2.IsNil()) {
            return v1.IsNil() == v2.IsNil();
        }
        return deepValueEqual(v1.Elem(), v2.Elem(), visited, depth + 1);
    } else if ((v1.Kind() == Ptr)) {
    _ident_4_:
        if (v1.Pointer() == v2.Pointer()) {
            return true;
        }
        return deepValueEqual(v1.Elem(), v2.Elem(), visited, depth + 1);
    } else if ((v1.Kind() == Struct)) {
    _ident_5_ : {
        int i{0};
        int n{0};
        for (std::tie(i, n) = std::tuple<int, int>(0, v1.NumField()); i < n;
             i++) {
            if (!deepValueEqual(v1.Field(i), v2.Field(i), visited, depth + 1)) {
                return false;
            }
        }
    }
        return true;
    } else if ((v1.Kind() == Map)) {
    _ident_6_:
        if (v1.IsNil() != v2.IsNil()) {
            return false;
        }
        if (v1.Len() != v2.Len()) {
            return false;
        }
        if (v1.Pointer() == v2.Pointer()) {
            return true;
        }
        {
            Value k{};
            for (k : moku::range_value<moku::slice<Value>>(v1.MapKeys())) {
                val1 = v1.MapIndex(k);
                val2 = v2.MapIndex(k);
                if (!val1.IsValid() || !val2.IsValid() ||
                    !deepValueEqual(v1.MapIndex(k),
                                    v2.MapIndex(k),
                                    visited,
                                    depth + 1)) {
                    return false;
                }
            }
        }
        return true;
    } else if ((v1.Kind() == Func)) {
    _ident_7_:
        if (v1.IsNil() && v2.IsNil()) {
            return true;
        }
        return false;
    } else {
    _ident_8_:
        return valueInterface(v1, false) == valueInterface(v2, false);
    }
}
bool DeepEqual(moku::interface x, moku::interface y)
{
    Value v1{};
    Value v2{};

    if (moku::is_nil(x) || moku::is_nil(y)) {
        return x == y;
    }
    v1 = ValueOf(moku::make_iface<moku::interface>(x));
    v2 = ValueOf(moku::make_iface<moku::interface>(y));
    if (v1.Type() != v2.Type()) {
        return false;
    }
    return deepValueEqual(v1, v2, make(std::map<visit, bool>), 0);
}
int main()
{
    kindNames = moku::slice<std::string>{{Invalid, "invalid"},
                                         {Bool, "bool"},
                                         {Int, "int"},
                                         {Int8, "int8"},
                                         {Int16, "int16"},
                                         {Int32, "int32"},
                                         {Int64, "int64"},
                                         {Uint, "uint"},
                                         {Uint8, "uint8"},
                                         {Uint16, "uint16"},
                                         {Uint32, "uint32"},
                                         {Uint64, "uint64"},
                                         {Uintptr, "uintptr"},
                                         {Float32, "float32"},
                                         {Float64, "float64"},
                                         {Complex64, "complex64"},
                                         {Complex128, "complex128"},
                                         {Array, "array"},
                                         {Chan, "chan"},
                                         {Func, "func"},
                                         {Interface, "interface"},
                                         {Map, "map"},
                                         {Ptr, "ptr"},
                                         {Slice, "slice"},
                                         {String, "string"},
                                         {Struct, "struct"},
                                         {UnsafePointer, "unsafe.Pointer"}};
    uint8Type = moku::type_assert<*rtype>(TypeOf(moku::make_iface<uint8_t>(
            &{uint8 %
              !s(token.Pos = 513956)[% !s(*ast.BasicLit = &{513957 5 0})] %
              !s(token.Pos = 0) % !s(token.Pos = 513958)})));
    _main();
    return 0;
}
Value MakeFunc(Type typ,
               std::function<moku::slice<Value>(moku::slice<Value>)> fn)
{
    uintptr_t code{0};
    std::function<void()> dummy{std::nullptr};
    funcType *ftyp{std::nullptr};
    makeFuncImpl *impl{std::nullptr};
    bitVector *stack{std::nullptr};
    rtype *t{std::nullptr};

    if (typ.Kind() != Func) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 359015) STRING
                     "reflect: call of MakeFunc with non-Func type"}));
    }
    t = typ.common();
    ftyp = (*funcType)(unsafe::Pointer(t));
    dummy = makeFuncStub;
    code = **(**uintptr_t)(unsafe::Pointer(&dummy));
    std::tie(_, _, _, stack, _) = funcLayout(t, nil);
    impl = &makeFuncImpl{{code, code}, {stack, stack}, {typ, ftyp}, {fn, fn}};
    return Value{t, unsafe::Pointer(impl), flag(Func)};
}
void makeFuncStub()
{
}
Value makeMethodValue(std::string op, Value v)
{
    uintptr_t code{0};
    std::function<void()> dummy{std::nullptr};
    flag fl{};
    rtype *funcType{std::nullptr};
    methodValue *fv{std::nullptr};
    Value rcvr{};
    bitVector *stack{std::nullptr};

    if (v.flag & flagMethod == 0) {
        panic(moku::make_iface<std::string>(&{
                % !s(token.Pos = 360722) STRING
                  "reflect: internal error: invalid use of makeMethodValue"}));
    }
    fl = v.flag & (flagRO | flagAddr | flagIndir);
    fl |= flag(v.typ->Kind());
    rcvr = Value{v.typ, v.ptr, fl};
    funcType = moku::type_assert<*rtype>(v.Type());
    dummy = methodValueCall;
    code = **(**uintptr_t)(unsafe::Pointer(&dummy));
    std::tie(_, _, _, stack, _) = funcLayout(funcType, nil);
    fv = &methodValue{{fn, code},
                      {stack, stack},
                      {method, int(v.flag) >> flagMethodShift},
                      {rcvr, rcvr}};
    methodReceiver(op, fv->rcvr, fv->method);
    return Value{funcType, unsafe::Pointer(fv), v.flag & flagRO | flag(Func)};
}
void methodValueCall()
{
}
int main()
{
    kindNames = moku::slice<std::string>{{Invalid, "invalid"},
                                         {Bool, "bool"},
                                         {Int, "int"},
                                         {Int8, "int8"},
                                         {Int16, "int16"},
                                         {Int32, "int32"},
                                         {Int64, "int64"},
                                         {Uint, "uint"},
                                         {Uint8, "uint8"},
                                         {Uint16, "uint16"},
                                         {Uint32, "uint32"},
                                         {Uint64, "uint64"},
                                         {Uintptr, "uintptr"},
                                         {Float32, "float32"},
                                         {Float64, "float64"},
                                         {Complex64, "complex64"},
                                         {Complex128, "complex128"},
                                         {Array, "array"},
                                         {Chan, "chan"},
                                         {Func, "func"},
                                         {Interface, "interface"},
                                         {Map, "map"},
                                         {Ptr, "ptr"},
                                         {Slice, "slice"},
                                         {String, "string"},
                                         {Struct, "struct"},
                                         {UnsafePointer, "unsafe.Pointer"}};
    uint8Type = moku::type_assert<*rtype>(TypeOf(moku::make_iface<uint8_t>(
            &{uint8 %
              !s(token.Pos = 513956)[% !s(*ast.BasicLit = &{513957 5 0})] %
              !s(token.Pos = 0) % !s(token.Pos = 513958)})));
    _main();
    return 0;
}
std::function<void(int, int)> Swapper(moku::interface slice)
{
    bool hasPtr{false};
    sliceHeader *s{std::nullptr};
    uintptr_t size{0};
    void *tmp{std::nullptr};
    rtype *typ{std::nullptr};
    Value v{};

    v = ValueOf(moku::make_iface<moku::interface>(slice));
    if (v.Kind() != Slice) {
        panic(moku::make_iface<ValueError *>(&{
                % !s(token.Pos = 352332) &
                % !s(*ast.CompositeLit =
                             &{0xc420666b80 352343
                                       [0xc4205447e0 0xc4205448a0] 352377})}));
    }
    if ((v.Len() == 0)) {
    _ident_0_:
        return [=](int i, int j) mutable -> void {
            panic(moku::make_iface<std::string>(
                    &{ % !s(token.Pos = 352501) STRING
                         "reflect: slice index out of range"}));
        };
    } else if ((v.Len() == 1)) {
    _ident_1_:
        return [=](int i, int j) mutable -> void {
            if (i != 0 || j != 0) {
                panic(moku::make_iface<std::string>(
                        &{ % !s(token.Pos = 352610) STRING
                             "reflect: slice index out of range"}));
            }
        };
    }
    typ = moku::type_assert<*rtype>(v.Type().Elem());
    size = typ->Size();
    hasPtr = typ->kind & kindNoPointers == 0;
    if (hasPtr) {
        if (size == ptrSize) {
            ps = *(*moku::slice<unsafe::Pointer>)(v.ptr);
            return [=](int i, int j) mutable -> void {
                std::tie(ps[i], ps[j]) =
                        std::tuple<void *, void *>(ps[j], ps[i]);
            };
        }
        if (typ->Kind() == String) {
            ss = *(*moku::slice<std::string>)(v.ptr);
            return [=](int i, int j) mutable -> void {
                std::tie(ss[i], ss[j]) =
                        std::tuple<std::string, std::string>(ss[j], ss[i]);
            };
        }
    } else {
        if ((size == 8)) {
        _ident_2_:
            is = *(*moku::slice<int64_t>)(v.ptr);
            return [=](int i, int j) mutable -> void {
                std::tie(is[i], is[j]) =
                        std::tuple<int64_t, int64_t>(is[j], is[i]);
            };
        } else if ((size == 4)) {
        _ident_3_:
            is = *(*moku::slice<int32_t>)(v.ptr);
            return [=](int i, int j) mutable -> void {
                std::tie(is[i], is[j]) =
                        std::tuple<int32_t, int32_t>(is[j], is[i]);
            };
        } else if ((size == 2)) {
        _ident_4_:
            is = *(*moku::slice<int16_t>)(v.ptr);
            return [=](int i, int j) mutable -> void {
                std::tie(is[i], is[j]) =
                        std::tuple<int16_t, int16_t>(is[j], is[i]);
            };
        } else if ((size == 1)) {
        _ident_5_:
            is = *(*moku::slice<int8_t>)(v.ptr);
            return [=](int i, int j) mutable -> void {
                std::tie(is[i], is[j]) =
                        std::tuple<int8_t, int8_t>(is[j], is[i]);
            };
        }
    }
    s = (*sliceHeader)(v.ptr);
    tmp = unsafe_New(typ);
    return [=](int i, int j) mutable -> void {
        if (unsigned int(i) >= unsigned int(s->Len) ||
            unsigned int(j) >= unsigned int(s->Len)) {
            panic(moku::make_iface<std::string>(
                    &{ % !s(token.Pos = 353638) STRING
                         "reflect: slice index out of range"}));
        }
        val1 = arrayAt(s->Data, i, size);
        val2 = arrayAt(s->Data, j, size);
        typedmemmove(typ, tmp, val1);
        typedmemmove(typ, val1, val2);
        typedmemmove(typ, val2, tmp);
    };
}
int main()
{
    kindNames = moku::slice<std::string>{{Invalid, "invalid"},
                                         {Bool, "bool"},
                                         {Int, "int"},
                                         {Int8, "int8"},
                                         {Int16, "int16"},
                                         {Int32, "int32"},
                                         {Int64, "int64"},
                                         {Uint, "uint"},
                                         {Uint8, "uint8"},
                                         {Uint16, "uint16"},
                                         {Uint32, "uint32"},
                                         {Uint64, "uint64"},
                                         {Uintptr, "uintptr"},
                                         {Float32, "float32"},
                                         {Float64, "float64"},
                                         {Complex64, "complex64"},
                                         {Complex128, "complex128"},
                                         {Array, "array"},
                                         {Chan, "chan"},
                                         {Func, "func"},
                                         {Interface, "interface"},
                                         {Map, "map"},
                                         {Ptr, "ptr"},
                                         {Slice, "slice"},
                                         {String, "string"},
                                         {Struct, "struct"},
                                         {UnsafePointer, "unsafe.Pointer"}};
    uint8Type = moku::type_assert<*rtype>(TypeOf(moku::make_iface<uint8_t>(
            &{uint8 %
              !s(token.Pos = 513956)[% !s(*ast.BasicLit = &{513957 5 0})] %
              !s(token.Pos = 0) % !s(token.Pos = 513958)})));
    _main();
    return 0;
}
uintptr_t structField::offset()
{
    return this->offsetAnon >> 1;
}
bool structField::anon()
{
    return this->offsetAnon & 1 != 0;
}
uint8_t *name::_dataByValue(int off)
{
    return (*byte)(add(unsafe::Pointer(this->bytes), uintptr_t(off)));
}
bool name::_isExportedByValue()
{
    return (*this->bytes) & (1 << 0) != 0;
}
int name::_nameLenByValue()
{
    return int(uint16_t(*this->data(1)) << 8 | uint16_t(*this->data(2)));
}
int name::_tagLenByValue()
{
    int off{0};

    if (*this->data(0) & (1 << 1) == 0) {
        return 0;
    }
    off = 3 + this->nameLen();
    return int(uint16_t(*this->data(off)) << 8 |
               uint16_t(*this->data(off + 1)));
}
std::string name::_nameByValue()
{
    std::vector<uint8_t> *b{std::nullptr};
    stringHeader *hdr{std::nullptr};
    std::string s{""};

    if (moku::is_nil(this->bytes)) {
        return;
    }
    b = (*std::vector<byte>)(unsafe::Pointer(this->bytes));
    hdr = (*stringHeader)(unsafe::Pointer(&s));
    hdr->Data = unsafe::Pointer(&b[3]);
    hdr->Len = int(b[1]) << 8 | int(b[2]);
    return s;
}
std::string name::_tagByValue()
{
    stringHeader *hdr{std::nullptr};
    int nl{0};
    std::string s{""};
    int tl{0};

    tl = this->tagLen();
    if (tl == 0) {
        return "";
    }
    nl = this->nameLen();
    hdr = (*stringHeader)(unsafe::Pointer(&s));
    hdr->Data = unsafe::Pointer(this->data(3 + nl + 2));
    hdr->Len = tl;
    return s;
}
std::string name::_pkgPathByValue()
{
    int32_t nameOff{0};
    int off{0};
    name pkgPathName{};

    if (moku::is_nil(this->bytes) || *this->data(0) & (1 << 2) == 0) {
        return "";
    }
    off = 3 + this->nameLen();
    {
        int tl{0};

        tl = this->tagLen();

        if (tl > 0) {
            off += 2 + tl;
        }
    }
    copy(moku::slice_expr<std::vector<uint8_t> *>(
                 (*std::vector<byte>)(unsafe::Pointer(&nameOff))),
         moku::slice_expr<std::vector<uint8_t> *>(
                 (*std::vector<byte>)(unsafe::Pointer(this->data(off)))));
    pkgPathName = name{
            (*byte)(resolveTypeOff(unsafe::Pointer(this->bytes), nameOff))};
    return pkgPathName.name();
}
uintptr_t round(uintptr_t n, uintptr_t a)
{
    return (n + a - 1) & ~((a - 1));
}
name newName(std::string n, std::string tag, std::string pkgPath, bool exported)
{
    moku::slice<uint8_t> b{};
    uint8_t bits{0};
    int l{0};

    if (len(n) > 1 << 16 - 1) {
        panic(moku::make_iface<std::string>(
                &{ %
                           !s(*ast.BasicLit = &{395721 9 "reflect.nameFrom: "
                                                         "name too long: "}) %
                           !s(token.Pos = 395757) +
                   n}));
    }
    if (len(tag) > 1 << 16 - 1) {
        panic(moku::make_iface<std::string>(
                &{ %
                           !s(*ast.BasicLit = &{395798 9 "reflect.nameFrom: "
                                                         "tag too long: "}) %
                           !s(token.Pos = 395833) +
                   tag}));
    }
    l = 1 + 2 + len(n);
    if (exported) {
        bits |= 1 << 0;
    }
    if (len(tag) > 0) {
        l += 2 + len(tag);
        bits |= 1 << 1;
    }
    if (pkgPath != "") {
        bits |= 1 << 2;
    }
    b = make(moku::slice<byte>, l);
    b[0] = bits;
    b[1] = uint8_t(len(n) >> 8);
    b[2] = uint8_t(len(n));
    copy(moku::slice_expr<moku::slice<uint8_t>>(b, 3), n);
    if (len(tag) > 0) {
        tb = moku::slice_expr<moku::slice<uint8_t>>(b, 3 + len(n));
        tb[0] = uint8_t(len(tag) >> 8);
        tb[1] = uint8_t(len(tag));
        copy(moku::slice_expr<moku::slice<uint8_t>>(tb, 2), tag);
    }
    if (pkgPath != "") {
        panic(moku::make_iface<std::string>(&{ % !s(token.Pos = 396264) STRING
                                                 "reflect: creating a name "
                                                 "with a package path is not "
                                                 "supported"}));
    }
    return name{{bytes, &b[0]}};
}
std::string Kind::_StringByValue()
{
    if (int(this) < len(kindNames)) {
        return kindNames[this];
    }
    return "kind" + strconv::Itoa(int(this));
}
moku::slice<method> uncommonType::methods()
{
    return moku::slice_expr<std::vector<method> *>(
            (*std::vector<method>)(add(unsafe::Pointer(this),
                                       uintptr_t(this->moff))),
            this->mcount,
            this->mcount);
}
void *resolveNameOff(void *ptrInModule, int32_t off)
{
}
void *resolveTypeOff(void *rtype, int32_t off)
{
}
void *resolveTextOff(void *rtype, int32_t off)
{
}
int32_t addReflectOff(void *ptr)
{
}
nameOff resolveReflectName(name n)
{
    return nameOff(addReflectOff(unsafe::Pointer(n.bytes)));
}
typeOff resolveReflectType(rtype *t)
{
    return typeOff(addReflectOff(unsafe::Pointer(t)));
}
textOff resolveReflectText(void *ptr)
{
    return textOff(addReflectOff(ptr));
}
name rtype::nameOff(nameOff off)
{
    return name{(*byte)(resolveNameOff(unsafe::Pointer(this), int32_t(off)))};
}
rtype *rtype::typeOff(typeOff off)
{
    return (*rtype)(resolveTypeOff(unsafe::Pointer(this), int32_t(off)));
}
void *rtype::textOff(textOff off)
{
    return resolveTextOff(unsafe::Pointer(this), int32_t(off));
}
uncommonType *rtype::uncommon()
{
    if (this->tflag & tflagUncommon == 0) {
        return nil;
    }
    if ((this->Kind() == Struct)) {
    _ident_0_:
        return &(*structTypeUncommon)(unsafe::Pointer(this))->u;
    } else if ((this->Kind() == Ptr)) {
    _ident_1_:
        return &(*u)(unsafe::Pointer(this))->u;
    } else if ((this->Kind() == Func)) {
    _ident_2_:
        return &(*u)(unsafe::Pointer(this))->u;
    } else if ((this->Kind() == Slice)) {
    _ident_3_:
        return &(*u)(unsafe::Pointer(this))->u;
    } else if ((this->Kind() == Array)) {
    _ident_4_:
        return &(*u)(unsafe::Pointer(this))->u;
    } else if ((this->Kind() == Chan)) {
    _ident_5_:
        return &(*u)(unsafe::Pointer(this))->u;
    } else if ((this->Kind() == Map)) {
    _ident_6_:
        return &(*u)(unsafe::Pointer(this))->u;
    } else if ((this->Kind() == Interface)) {
    _ident_7_:
        return &(*u)(unsafe::Pointer(this))->u;
    } else {
    _ident_8_:
        return &(*u)(unsafe::Pointer(this))->u;
    }
}
std::string rtype::String()
{
    std::string s{""};

    s = this->nameOff(this->str).name();
    if (this->tflag & tflagExtraStar != 0) {
        return moku::slice_expr<std::string>(s, 1);
    }
    return s;
}
uintptr_t rtype::Size()
{
    return this->size;
}
int rtype::Bits()
{
    Kind k{};

    if (moku::is_nil(this)) {
        panic(moku::make_iface<std::string>(&{ % !s(token.Pos = 401661) STRING
                                                 "reflect: Bits of nil Type"}));
    }
    k = this->Kind();
    if (k < Int || k > Complex128) {
        panic(moku::make_iface<std::string>(&{
                %
                        !s(*ast.BasicLit = &{401748 9 "reflect: Bits of "
                                                      "non-arithmetic Type "}) %
                        !s(token.Pos = 401788) +
                % !s(*ast.CallExpr = &{0xc420632a80 401798 [] 0 401799})}));
    }
    return int(this->size) * 8;
}
int rtype::Align()
{
    return int(this->align);
}
int rtype::FieldAlign()
{
    return int(this->fieldAlign);
}
Kind rtype::Kind()
{
    return Kind(this->kind & kindMask);
}
bool rtype::pointers()
{
    return this->kind & kindNoPointers == 0;
}
rtype *rtype::common()
{
    return this;
}
moku::slice<method> rtype::exportedMethods()
{
    bool allExported{false};
    moku::slice<method> allm{};
    bool found{false};
    moku::slice<method> methods{};
    moku::interface methodsi{};
    uncommonType *ut{std::nullptr};

    std::tie(methodsi, found) = methodCache.Load(moku::make_iface<rtype *>(t));
    if (found) {
        return moku::try_assert<moku::slice<method>>(methodsi);
    }
    ut = this->uncommon();
    if (moku::is_nil(ut)) {
        return nil;
    }
    allm = ut->methods();
    allExported = true;
    {
        method m{};
        for (m : moku::range_value<moku::slice<method>>(allm)) {
            name = this->nameOff(m.name);
            if (!name.isExported()) {
                allExported = false;
                break;
            }
        }
    }
    if (allExported) {
        methods = allm;
    } else {
        methods = make(moku::slice<method>, 0, len(allm));
        {
            method m{};
            for (m : moku::range_value<moku::slice<method>>(allm)) {
                name = this->nameOff(m.name);
                if (name.isExported()) {
                    methods = append(methods, m);
                }
            }
        }
        methods = moku::slice_expr<moku::slice<method>>(
                methods, len(methods), len(methods));
    }
    std::tie(methodsi, _) = methodCache.LoadOrStore(
            moku::make_iface<rtype *>(t),
            moku::make_iface<moku::slice<method>>(methods));
    return moku::try_assert<moku::slice<method>>(methodsi);
}
int rtype::NumMethod()
{
    if (this->Kind() == Interface) {
        tt = (*interfaceType)(unsafe::Pointer(this));
        return tt->NumMethod();
    }
    if (this->tflag & tflagUncommon == 0) {
        return 0;
    }
    return len(this->exportedMethods());
}
Method rtype::Method(int i)
{
    flag fl{};
    void *fn{std::nullptr};
    funcType *ft{std::nullptr};
    moku::slice<Type> in{};
    Method m{};
    moku::slice<method> methods{};
    Type mt{};
    rtype *mtyp{std::nullptr};
    moku::slice<Type> out{};
    method p{};
    name pname{};
    void *tfn{std::nullptr};

    if (this->Kind() == Interface) {
        tt = (*interfaceType)(unsafe::Pointer(this));
        return tt->Method(i);
    }
    methods = this->exportedMethods();
    if (i < 0 || i >= len(methods)) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 403354) STRING
                     "reflect: Method index out of range"}));
    }
    p = methods[i];
    pname = this->nameOff(p.name);
    m.Name = pname.name();
    fl = flag(Func);
    mtyp = this->typeOff(p.mtyp);
    ft = (*funcType)(unsafe::Pointer(mtyp));
    in = make(moku::slice<Type>, 0, 1 + len(ft->in()));
    in = append(in, this);
    {
        rtype *arg{std::nullptr};
        for (arg : moku::range_value<moku::slice<rtype *>>(ft->in())) {
            in = append(in, arg);
        }
    }
    out = make(moku::slice<Type>, 0, len(ft->out()));
    {
        rtype *ret{std::nullptr};
        for (ret : moku::range_value<moku::slice<rtype *>>(ft->out())) {
            out = append(out, ret);
        }
    }
    mt = FuncOf(in, out, ft->IsVariadic());
    m.Type = mt;
    tfn = this->textOff(p.tfn);
    fn = unsafe::Pointer(&tfn);
    m.Func = Value{moku::type_assert<*rtype>(mt), fn, fl};
    m.Index = i;
    return m;
}
std::tuple<Method, bool> rtype::MethodByName(std::string name)
{
    Method m{};
    bool ok{false};
    uncommonType *ut{std::nullptr};
    moku::slice<method> utmethods{};

    if (this->Kind() == Interface) {
        tt = (*interfaceType)(unsafe::Pointer(this));
        return tt->MethodByName(name);
    }
    ut = this->uncommon();
    if (moku::is_nil(ut)) {
        return {Method{}, false};
    }
    utmethods = ut->methods();
    {
        int i{0};
        for (i = 0; i < int(ut->mcount); i++) {
            method p{};
            name pname{};

            p = utmethods[i];
            pname = this->nameOff(p.name);
            if (pname.isExported() && pname.name() == name) {
                return {this->Method(i), true};
            }
        }
    }
    return {Method{}, false};
}
std::string rtype::PkgPath()
{
    uncommonType *ut{std::nullptr};

    if (this->tflag & tflagNamed == 0) {
        return "";
    }
    ut = this->uncommon();
    if (moku::is_nil(ut)) {
        return "";
    }
    return this->nameOff(ut->pkgPath).name();
}
bool hasPrefix(std::string s, std::string prefix)
{
    return len(s) >= len(prefix) &&
           moku::slice_expr<std::string>(s, len(prefix)) == prefix;
}
std::string rtype::Name()
{
    int i{0};
    std::string s{""};

    if (this->tflag & tflagNamed == 0) {
        return "";
    }
    s = this->String();
    i = len(s) - 1;
    while (i >= 0) {
        if (s[i] == '.') {
            break;
        }
        i--;
    }
    return moku::slice_expr<std::string>(s, i + 1);
}
ChanDir rtype::ChanDir()
{
    chanType *tt{std::nullptr};

    if (this->Kind() != Chan) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 404920) STRING
                     "reflect: ChanDir of non-chan type"}));
    }
    tt = (*chanType)(unsafe::Pointer(this));
    return ChanDir(tt->dir);
}
bool rtype::IsVariadic()
{
    funcType *tt{std::nullptr};

    if (this->Kind() != Func) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 405092) STRING
                     "reflect: IsVariadic of non-func type"}));
    }
    tt = (*funcType)(unsafe::Pointer(this));
    return tt->outCount & (1 << 15) != 0;
}
Type rtype::Elem()
{
    if ((this->Kind() == Array)) {
    _ident_9_:
        tt = (*arrayType)(unsafe::Pointer(this));
        return toType(tt->elem);
    } else if ((this->Kind() == Chan)) {
    _ident_10_:
        tt = (*chanType)(unsafe::Pointer(this));
        return toType(tt->elem);
    } else if ((this->Kind() == Map)) {
    _ident_11_:
        tt = (*mapType)(unsafe::Pointer(this));
        return toType(tt->elem);
    } else if ((this->Kind() == Ptr)) {
    _ident_12_:
        tt = (*ptrType)(unsafe::Pointer(this));
        return toType(tt->elem);
    } else if ((this->Kind() == Slice)) {
    _ident_13_:
        tt = (*sliceType)(unsafe::Pointer(this));
        return toType(tt->elem);
    }
    panic(moku::make_iface<std::string>(&{ % !s(token.Pos = 405648) STRING
                                             "reflect: Elem of invalid type"}));
}
StructField rtype::Field(int i)
{
    structType *tt{std::nullptr};

    if (this->Kind() != Struct) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 405760) STRING
                     "reflect: Field of non-struct type"}));
    }
    tt = (*structType)(unsafe::Pointer(this));
    return tt->Field(i);
}
StructField rtype::FieldByIndex(moku::slice<int> index)
{
    structType *tt{std::nullptr};

    if (this->Kind() != Struct) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 405952) STRING
                     "reflect: FieldByIndex of non-struct type"}));
    }
    tt = (*structType)(unsafe::Pointer(this));
    return tt->FieldByIndex(index);
}
std::tuple<StructField, bool> rtype::FieldByName(std::string name)
{
    structType *tt{std::nullptr};

    if (this->Kind() != Struct) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 406169) STRING
                     "reflect: FieldByName of non-struct type"}));
    }
    tt = (*structType)(unsafe::Pointer(this));
    return tt->FieldByName(name);
}
std::tuple<StructField, bool> rtype::FieldByNameFunc(
        std::function<bool(std::string)> match)
{
    structType *tt{std::nullptr};

    if (this->Kind() != Struct) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 406399) STRING
                     "reflect: FieldByNameFunc of non-struct type"}));
    }
    tt = (*structType)(unsafe::Pointer(this));
    return tt->FieldByNameFunc(match);
}
Type rtype::In(int i)
{
    funcType *tt{std::nullptr};

    if (this->Kind() != Func) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 406590) STRING
                     "reflect: In of non-func type"}));
    }
    tt = (*funcType)(unsafe::Pointer(this));
    return toType(tt->in()[i]);
}
Type rtype::Key()
{
    mapType *tt{std::nullptr};

    if (this->Kind() != Map) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 406752) STRING
                     "reflect: Key of non-map type"}));
    }
    tt = (*mapType)(unsafe::Pointer(this));
    return toType(tt->key);
}
int rtype::Len()
{
    arrayType *tt{std::nullptr};

    if (this->Kind() != Array) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 406910) STRING
                     "reflect: Len of non-array type"}));
    }
    tt = (*arrayType)(unsafe::Pointer(this));
    return int(tt->len);
}
int rtype::NumField()
{
    structType *tt{std::nullptr};

    if (this->Kind() != Struct) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 407075) STRING
                     "reflect: NumField of non-struct type"}));
    }
    tt = (*structType)(unsafe::Pointer(this));
    return len(tt->fields);
}
int rtype::NumIn()
{
    funcType *tt{std::nullptr};

    if (this->Kind() != Func) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 407245) STRING
                     "reflect: NumIn of non-func type"}));
    }
    tt = (*funcType)(unsafe::Pointer(this));
    return int(tt->inCount);
}
int rtype::NumOut()
{
    funcType *tt{std::nullptr};

    if (this->Kind() != Func) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 407410) STRING
                     "reflect: NumOut of non-func type"}));
    }
    tt = (*funcType)(unsafe::Pointer(this));
    return len(tt->out());
}
Type rtype::Out(int i)
{
    funcType *tt{std::nullptr};

    if (this->Kind() != Func) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 407577) STRING
                     "reflect: Out of non-func type"}));
    }
    tt = (*funcType)(unsafe::Pointer(this));
    return toType(tt->out()[i]);
}
moku::slice<rtype *> funcType::in()
{
    uintptr_t uadd{0};

    uadd = unsafe::Sizeof(*this);
    if (this->tflag & tflagUncommon != 0) {
        uadd += unsafe::Sizeof(uncommonType{});
    }
    return moku::slice_expr<std::vector<rtype *> *>(
            (*std::vector<*rtype>)(add(unsafe::Pointer(this), uadd)),
            this->inCount);
}
moku::slice<rtype *> funcType::out()
{
    uint16_t outCount{0};
    uintptr_t uadd{0};

    uadd = unsafe::Sizeof(*this);
    if (this->tflag & tflagUncommon != 0) {
        uadd += unsafe::Sizeof(uncommonType{});
    }
    outCount = this->outCount & (1 << 15 - 1);
    return moku::slice_expr<std::vector<rtype *> *>(
            (*std::vector<*rtype>)(add(unsafe::Pointer(this), uadd)),
            this->inCount,
            this->inCount + outCount);
}
void *add(void *p, uintptr_t x)
{
    return unsafe::Pointer(uintptr_t(p) + x);
}
std::string ChanDir::_StringByValue()
{
    if ((this == SendDir)) {
    _ident_14_:
        return "chan<-";
    } else if ((this == RecvDir)) {
    _ident_15_:
        return "<-chan";
    } else if ((this == BothDir)) {
    _ident_16_:
        return "chan";
    }
    return "ChanDir" + strconv::Itoa(int(this));
}
Method interfaceType::Method(int i)
{
    Method m{};
    imethod *p{std::nullptr};
    name pname{};

    if (i < 0 || i >= len(this->methods)) {
        return;
    }
    p = &this->methods[i];
    pname = this->nameOff(p->name);
    m.Name = pname.name();
    if (!pname.isExported()) {
        m.PkgPath = pname.pkgPath();
        if (m.PkgPath == "") {
            m.PkgPath = this->pkgPath.name();
        }
    }
    m.Type = toType(this->typeOff(p->typ));
    m.Index = i;
    return;
}
int interfaceType::NumMethod()
{
    return len(this->methods);
}
std::tuple<Method, bool> interfaceType::MethodByName(std::string name)
{
    Method m{};
    bool ok{false};
    imethod *p{std::nullptr};

    if (moku::is_nil(this)) {
        return;
    }
    {
        int i{0};
        for (i : moku::range_key<moku::slice<imethod>>(this->methods)) {
            p = &this->methods[i];
            if (this->nameOff(p->name).name() == name) {
                return {this->Method(i), true};
            }
        }
    }
    return;
}
std::string StructTag::_GetByValue(std::string key)
{
    std::string v{""};

    std::tie(v, _) = this->Lookup(key);
    return v;
}
std::tuple<std::string, bool> StructTag::_LookupByValue(std::string key)
{
    bool ok{false};
    std::string value{""};

    while (this != "") {
        int i{0};
        std::string name{""};
        std::string qvalue{""};

        i = 0;
        while (i < len(this) && this[i] == ' ') {
            i++;
        }
        this = moku::slice_expr<StructTag>(this, i);
        if (this == "") {
            break;
        }
        i = 0;
        while (i < len(this) && this[i] > ' ' && this[i] != ':' &&
               this[i] != '"' && this[i] != 0x7f) {
            i++;
        }
        if (i == 0 || i + 1 >= len(this) || this[i] != ':' ||
            this[i + 1] != '"') {
            break;
        }
        name = std::string(moku::slice_expr<StructTag>(this, i));
        this = moku::slice_expr<StructTag>(this, i + 1);
        i = 1;
        while (i < len(this) && this[i] != '"') {
            if (this[i] == '\\') {
                i++;
            }
            i++;
        }
        if (i >= len(this)) {
            break;
        }
        qvalue = std::string(moku::slice_expr<StructTag>(this, i + 1));
        this = moku::slice_expr<StructTag>(this, i + 1);
        if (key == name) {
            std::tie(value, err) = strconv::Unquote(qvalue);
            if (!moku::is_nil(err)) {
                break;
            }
            return {value, true};
        }
    }
    return {"", false};
}
StructField structType::Field(int i)
{
    StructField f{};
    structField *p{std::nullptr};

    if (i < 0 || i >= len(this->fields)) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 412453) STRING
                     "reflect: Field index out of bounds"}));
    }
    p = &this->fields[i];
    f.Type = toType(p->typ);
    f.Name = p->name.name();
    f.Anonymous = p->anon();
    if (!p->name.isExported()) {
        f.PkgPath = p->name.pkgPath();
        if (f.PkgPath == "") {
            f.PkgPath = this->pkgPath.name();
        }
    }
    {
        std::string tag{""};

        tag = p->name.tag();

        if (tag != "") {
            f.Tag = StructTag(tag);
        }
    }
    f.Offset = p->offset();
    f.Index = moku::slice<int>{i};
    return;
}
StructField structType::FieldByIndex(moku::slice<int> index)
{
    StructField f{};

    f.Type = toType(&this->rtype);
    {
        int i{0};
        int x{0};
        for (std::tie(i, x) : moku::range_key_value<moku::slice<int>>(index)) {
            if (i > 0) {
                ft = f.Type;
                if (ft.Kind() == Ptr && ft.Elem().Kind() == Struct) {
                    ft = ft.Elem();
                }
                f.Type = ft;
            }
            f = f.Type.Field(x);
        }
    }
    return;
}
std::tuple<StructField, bool> structType::FieldByNameFunc(
        std::function<bool(std::string)> match)
{
    moku::slice<fieldScan> current{};
    moku::slice<fieldScan> next{};
    std::map<structType *, int> nextCount{};
    bool ok{false};
    StructField result{};
    std::map<structType *, bool> visited{};

    current = moku::slice<fieldScan>{};
    next = moku::slice<fieldScan>{{{typ, this}}};
    visited = std::map<*structType, bool>{};
    while (len(next) > 0) {
        std::map<structType *, int> count{};

        std::tie(current, next) =
                std::tuple<moku::slice<fieldScan>, moku::slice<fieldScan>>(
                        next,
                        moku::slice_expr<moku::slice<fieldScan>>(current, 0));
        count = nextCount;
        nextCount = nil;
        {
            fieldScan scan{};
            for (scan : moku::range_value<moku::slice<fieldScan>>(current)) {
                this = scan.typ;
                if (visited[this]) {
                    continue;
                }
                visited[this] = true;
                {
                    int i{0};
                    for (i : moku::range_key<moku::slice<structField>>(
                                 this->fields)) {
                        f = &this->fields[i];
                        fname = f->name.name();
                        if (f->anon()) {
                            ntyp = f->typ;
                            if (ntyp->Kind() == Ptr) {
                                ntyp = ntyp->Elem().common();
                            }
                        }
                        if (match(fname)) {
                            if (count[this] > 1 || ok) {
                                return {StructField{}, false};
                            }
                            result = this->Field(i);
                            result.Index = nil;
                            result.Index = append(result.Index, scan.index);
                            result.Index = append(result.Index, i);
                            ok = true;
                            continue;
                        }
                        if (ok || moku::is_nil(ntyp) ||
                            ntyp->Kind() != Struct) {
                            continue;
                        }
                        styp = (*structType)(unsafe::Pointer(ntyp));
                        if (nextCount[styp] > 0) {
                            nextCount[styp] = 2;
                            continue;
                        }
                        if (moku::is_nil(nextCount)) {
                            nextCount = std::map<*structType, int>{};
                        }
                        nextCount[styp] = 1;
                        if (count[this] > 1) {
                            nextCount[styp] = 2;
                        }
                        index = append(index, scan.index);
                        index = append(index, i);
                        next = append(next, fieldScan{styp, index});
                    }
                }
            }
        }
        if (ok) {
            break;
        }
    }
    return;
}
std::tuple<StructField, bool> structType::FieldByName(std::string name)
{
    StructField f{};
    bool hasAnon{false};
    bool present{false};

    hasAnon = false;
    if (name != "") {
        {
            int i{0};
            for (i : moku::range_key<moku::slice<structField>>(this->fields)) {
                tf = &this->fields[i];
                if (tf->name.name() == name) {
                    return {this->Field(i), true};
                }
                if (tf->anon()) {
                    hasAnon = true;
                }
            }
        }
    }
    if (!hasAnon) {
        return;
    }
    return this->FieldByNameFunc(
            [=](std::string s) mutable -> bool { return s == name; });
}
Type TypeOf(moku::interface i)
{
    emptyInterface eface{};

    eface = *(*emptyInterface)(unsafe::Pointer(&i));
    return toType(eface.typ);
}
Type PtrTo(Type t)
{
    return moku::type_assert<*rtype>(t)->ptrTo();
}
rtype *rtype::ptrTo()
{
    moku::interface iptr{};
    moku::interface pi{};
    ptrType pp{};
    ptrType *prototype{std::nullptr};
    std::string s{""};

    if (this->ptrToThis != 0) {
        return this->typeOff(this->ptrToThis);
    }
    {
        bool ok{false};
        moku::interface pi{};

        std::tie(pi, ok) = ptrMap.Load(moku::make_iface<rtype *>(t));

        if (ok) {
            return &moku::try_assert<*ptrType>(pi)->rtype;
        }
    }
    s = "*" + this->String();
    {
        rtype *tt{std::nullptr};
        for (tt : moku::range_value<moku::slice<rtype *>>(typesByString(s))) {
            p = (*ptrType)(unsafe::Pointer(tt));
            if (p->elem != this) {
                continue;
            }
            std::tie(pi, _) =
                    ptrMap.LoadOrStore(moku::make_iface<rtype *>(t),
                                       moku::make_iface<ptrType *>(p));
            return &moku::try_assert<*ptrType>(pi)->rtype;
        }
    }
    prototype = *(**ptrType)(unsafe::Pointer(&iptr));
    pp = *prototype;
    pp.str = resolveReflectName(newName(s, "", "", false));
    pp.ptrToThis = 0;
    pp.hash = fnv1(this->hash, '*');
    pp.elem = this;
    std::tie(pi, _) = ptrMap.LoadOrStore(
            moku::make_iface<rtype *>(t),
            moku::make_iface<ptrType *>(&{ % !s(token.Pos = 419677) & pp}));
    return &moku::try_assert<*ptrType>(pi)->rtype;
}
uint32_t fnv1(uint32_t x, moku::slice<uint8_t> list)
{
    {
        uint8_t b{0};
        for (b : moku::range_value<moku::slice<uint8_t>>(list)) {
            x = x * 16777619 ^ uint32_t(b);
        }
    }
    return x;
}
bool rtype::Implements(Type u)
{
    if (moku::is_nil(u)) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 419979) STRING
                     "reflect: nil type passed to Type.Implements"}));
    }
    if (u.Kind() != Interface) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 420065) STRING
                     "reflect: non-interface type passed to Type.Implements"}));
    }
    return implements(moku::type_assert<*rtype>(u), this);
}
bool rtype::AssignableTo(Type u)
{
    rtype *uu{std::nullptr};

    if (moku::is_nil(u)) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 420229) STRING
                     "reflect: nil type passed to Type.AssignableTo"}));
    }
    uu = moku::type_assert<*rtype>(u);
    return directlyAssignable(uu, this) || implements(uu, this);
}
bool rtype::ConvertibleTo(Type u)
{
    rtype *uu{std::nullptr};

    if (moku::is_nil(u)) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 420425) STRING
                     "reflect: nil type passed to Type.ConvertibleTo"}));
    }
    uu = moku::type_assert<*rtype>(u);
    return !moku::is_nil(convertOp(uu, this));
}
bool rtype::Comparable()
{
    return !moku::is_nil(this->alg) && !moku::is_nil(this->alg->equal);
}
bool implements(rtype *T, rtype *V)
{
    int i{0};
    interfaceType *t{std::nullptr};
    uncommonType *v{std::nullptr};
    moku::slice<method> vmethods{};

    if (T->Kind() != Interface) {
        return false;
    }
    t = (*interfaceType)(unsafe::Pointer(T));
    if (len(t->methods) == 0) {
        return true;
    }
    if (V->Kind() == Interface) {
        v = (*interfaceType)(unsafe::Pointer(V));
        i = 0;
        {
            int j{0};
            for (j = 0; j < len(v->methods); j++) {
                imethod *tm{std::nullptr};
                name tmName{};
                imethod *vm{std::nullptr};
                name vmName{};

                tm = &t->methods[i];
                tmName = t->nameOff(tm->name);
                vm = &v->methods[j];
                vmName = V->nameOff(vm->name);
                if (vmName.name() == tmName.name() &&
                    V->typeOff(vm->typ) == t->typeOff(tm->typ)) {
                    if (!tmName.isExported()) {
                        tmPkgPath = tmName.pkgPath();
                        if (tmPkgPath == "") {
                            tmPkgPath = t->pkgPath.name();
                        }
                        vmPkgPath = vmName.pkgPath();
                        if (vmPkgPath == "") {
                            vmPkgPath = v->pkgPath.name();
                        }
                        if (tmPkgPath != vmPkgPath) {
                            continue;
                        }
                    }
                    {
                        i++;

                        if (i >= len(t->methods)) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
    v = V->uncommon();
    if (moku::is_nil(v)) {
        return false;
    }
    i = 0;
    vmethods = v->methods();
    {
        int j{0};
        for (j = 0; j < int(v->mcount); j++) {
            imethod *tm{std::nullptr};
            name tmName{};
            method vm{};
            name vmName{};

            tm = &t->methods[i];
            tmName = t->nameOff(tm->name);
            vm = vmethods[j];
            vmName = V->nameOff(vm.name);
            if (vmName.name() == tmName.name() &&
                V->typeOff(vm.mtyp) == t->typeOff(tm->typ)) {
                if (!tmName.isExported()) {
                    tmPkgPath = tmName.pkgPath();
                    if (tmPkgPath == "") {
                        tmPkgPath = t->pkgPath.name();
                    }
                    vmPkgPath = vmName.pkgPath();
                    if (vmPkgPath == "") {
                        vmPkgPath = V->nameOff(v->pkgPath).name();
                    }
                    if (tmPkgPath != vmPkgPath) {
                        continue;
                    }
                }
                {
                    i++;

                    if (i >= len(t->methods)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
bool directlyAssignable(rtype *T, rtype *V)
{
    if (T == V) {
        return true;
    }
    if (T->Name() != "" && V->Name() != "" || T->Kind() != V->Kind()) {
        return false;
    }
    return haveIdenticalUnderlyingType(T, V, true);
}
bool haveIdenticalType(Type T, Type V, bool cmpTags)
{
    if (cmpTags) {
        return T == V;
    }
    if (T.Name() != V.Name() || T.Kind() != V.Kind()) {
        return false;
    }
    return haveIdenticalUnderlyingType(T.common(), V.common(), false);
}
bool haveIdenticalUnderlyingType(rtype *T, rtype *V, bool cmpTags)
{
    Kind kind{};

    if (T == V) {
        return true;
    }
    kind = T->Kind();
    if (kind != V->Kind()) {
        return false;
    }
    if (Bool <= kind && kind <= Complex128 || kind == String ||
        kind == UnsafePointer) {
        return true;
    }
    if ((kind == Array)) {
    _ident_17_:
        return T->Len() == V->Len() &&
               haveIdenticalType(T->Elem(), V->Elem(), cmpTags);
    } else if ((kind == Chan)) {
    _ident_18_:
        if (V->ChanDir() == BothDir &&
            haveIdenticalType(T->Elem(), V->Elem(), cmpTags)) {
            return true;
        }
        return V->ChanDir() == T->ChanDir() &&
               haveIdenticalType(T->Elem(), V->Elem(), cmpTags);
    } else if ((kind == Func)) {
    _ident_19_:
        t = (*funcType)(unsafe::Pointer(T));
        v = (*funcType)(unsafe::Pointer(V));
        if (t->outCount != v->outCount || t->inCount != v->inCount) {
            return false;
        }
        {
            int i{0};
            for (i = 0; i < t->NumIn(); i++) {
                if (!haveIdenticalType(t->In(i), v->In(i), cmpTags)) {
                    return false;
                }
            }
        }
        {
            int i{0};
            for (i = 0; i < t->NumOut(); i++) {
                if (!haveIdenticalType(t->Out(i), v->Out(i), cmpTags)) {
                    return false;
                }
            }
        }
        return true;
    } else if ((kind == Interface)) {
    _ident_20_:
        t = (*interfaceType)(unsafe::Pointer(T));
        v = (*interfaceType)(unsafe::Pointer(V));
        if (len(t->methods) == 0 && len(v->methods) == 0) {
            return true;
        }
        return false;
    } else if ((kind == Map)) {
    _ident_21_:
        return haveIdenticalType(T->Key(), V->Key(), cmpTags) &&
               haveIdenticalType(T->Elem(), V->Elem(), cmpTags);
    } else if ((kind == Ptr) || (kind == Slice)) {
    _ident_22_:
        return haveIdenticalType(T->Elem(), V->Elem(), cmpTags);
    } else if ((kind == Struct)) {
    _ident_23_:
        t = (*structType)(unsafe::Pointer(T));
        v = (*structType)(unsafe::Pointer(V));
        if (len(t->fields) != len(v->fields)) {
            return false;
        }
        {
            int i{0};
            for (i : moku::range_key<moku::slice<structField>>(t->fields)) {
                tf = &t->fields[i];
                vf = &v->fields[i];
                if (tf->name.name() != vf->name.name()) {
                    return false;
                }
                if (!haveIdenticalType(tf->typ, vf->typ, cmpTags)) {
                    return false;
                }
                if (cmpTags && tf->name.tag() != vf->name.tag()) {
                    return false;
                }
                if (tf->offsetAnon != vf->offsetAnon) {
                    return false;
                }
                if (!tf->name.isExported()) {
                    tp = tf->name.pkgPath();
                    if (tp == "") {
                        tp = t->pkgPath.name();
                    }
                    vp = vf->name.pkgPath();
                    if (vp == "") {
                        vp = v->pkgPath.name();
                    }
                    if (tp != vp) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    return false;
}
std::tuple<moku::slice<void *>, moku::slice<moku::slice<int32_t>>> typelinks()
{
    moku::slice<moku::slice<int32_t>> offset{};
    moku::slice<void *> sections{};
}
rtype *rtypeOff(void *section, int32_t off)
{
    return (*rtype)(add(section, uintptr_t(off)));
}
moku::slice<rtype *> typesByString(std::string s)
{
    moku::slice<moku::slice<int32_t>> offset{};
    moku::slice<rtype *> ret{};
    moku::slice<void *> sections{};

    std::tie(sections, offset) = typelinks();
    {
        moku::slice<int32_t> offs{};
        int offsI{0};
        for (std::tie(offsI, offs) :
             moku::range_key_value<moku::slice<moku::slice<int32_t>>>(offset)) {
            section = sections[offsI];
            std::tie(i, j) = std::tuple<int, int>(0, len(offs));
            while (i < j) {
                int h{0};

                h = i + (j - i) / 2;
                if (!(rtypeOff(section, offs[h])->String() >= s)) {
                    i = h + 1;
                } else {
                    j = h;
                }
            }
            {
                int j{0};
                for (j = i; j < len(offs); j++) {
                    rtype *typ{std::nullptr};

                    typ = rtypeOff(section, offs[j]);
                    if (typ->String() != s) {
                        break;
                    }
                    ret = append(ret, typ);
                }
            }
        }
    }
    return ret;
}
Type ChanOf(ChanDir dir, Type t)
{
    chanType ch{};
    cacheKey ckey{};
    moku::interface ichan{};
    chanType *prototype{std::nullptr};
    std::string s{""};
    moku::interface ti{};
    rtype *typ{std::nullptr};

    typ = moku::type_assert<*rtype>(t);
    ckey = cacheKey{Chan, typ, nil, uintptr_t(dir)};
    {
        moku::interface ch{};
        bool ok{false};

        std::tie(ch, ok) = lookupCache.Load(moku::make_iface<cacheKey>(ckey));

        if (ok) {
            return moku::try_assert<*rtype>(ch);
        }
    }
    if (typ->size >= 1 << 16) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 429722) STRING
                     "reflect.ChanOf: element size too large"}));
    }
    if ((dir == SendDir)) {
    _ident_25_:
        s = "chan<- " + typ->String();
    } else if ((dir == RecvDir)) {
    _ident_26_:
        s = "<-chan " + typ->String();
    } else if ((dir == BothDir)) {
    _ident_27_:
        s = "chan " + typ->String();
    } else {
    _ident_24_:
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 429886) STRING
                     "reflect.ChanOf: invalid dir"}));
    }
    {
        rtype *tt{std::nullptr};
        for (tt : moku::range_value<moku::slice<rtype *>>(typesByString(s))) {
            ch = (*chanType)(unsafe::Pointer(tt));
            if (ch->elem == typ && ch->dir == uintptr_t(dir)) {
                std::tie(ti, _) = lookupCache.LoadOrStore(
                        moku::make_iface<cacheKey>(ckey),
                        moku::make_iface<rtype *>(tt));
                return moku::try_assert<Type>(ti);
            }
        }
    }
    prototype = *(**chanType)(unsafe::Pointer(&ichan));
    ch = *prototype;
    ch.tflag = 0;
    ch.dir = uintptr_t(dir);
    ch.str = resolveReflectName(newName(s, "", "", false));
    ch.hash = fnv1(typ->hash, 'c', byte(dir));
    ch.elem = typ;
    std::tie(ti, _) = lookupCache.LoadOrStore(
            moku::make_iface<cacheKey>(ckey),
            moku::make_iface<rtype *>(&{
                    % !s(token.Pos = 430595) &
                    % !s(*ast.SelectorExpr = &{0xc420d7b200 0xc420d7b220})}));
    return moku::try_assert<Type>(ti);
}
bool ismapkey(rtype *)
{
}
Type MapOf(Type key, Type elem)
{
    cacheKey ckey{};
    rtype *etyp{std::nullptr};
    moku::interface imap{};
    rtype *ktyp{std::nullptr};
    mapType mt{};
    std::string s{""};
    moku::interface ti{};

    ktyp = moku::type_assert<*rtype>(key);
    etyp = moku::type_assert<*rtype>(elem);
    if (!ismapkey(ktyp)) {
        panic(moku::make_iface<std::string>(
                &{ %
                           !s(*ast.BasicLit = &{431083 9 "reflect.MapOf: "
                                                         "invalid key type "}) %
                           !s(token.Pos = 431118) +
                   % !s(*ast.CallExpr = &{0xc420d7b680 431131 [] 0 431132})}));
    }
    ckey = cacheKey{Map, ktyp, etyp, 0};
    {
        moku::interface mt{};
        bool ok{false};

        std::tie(mt, ok) = lookupCache.Load(moku::make_iface<cacheKey>(ckey));

        if (ok) {
            return moku::try_assert<Type>(mt);
        }
    }
    s = "map[" + ktyp->String() + "]" + etyp->String();
    {
        rtype *tt{std::nullptr};
        for (tt : moku::range_value<moku::slice<rtype *>>(typesByString(s))) {
            mt = (*mapType)(unsafe::Pointer(tt));
            if (mt->key == ktyp && mt->elem == etyp) {
                std::tie(ti, _) = lookupCache.LoadOrStore(
                        moku::make_iface<cacheKey>(ckey),
                        moku::make_iface<rtype *>(tt));
                return moku::try_assert<Type>(ti);
            }
        }
    }
    mt = **(**mapType)(unsafe::Pointer(&imap));
    mt.str = resolveReflectName(newName(s, "", "", false));
    mt.tflag = 0;
    mt.hash = fnv1(etyp->hash,
                   'm',
                   byte(ktyp->hash >> 24),
                   byte(ktyp->hash >> 16),
                   byte(ktyp->hash >> 8),
                   byte(ktyp->hash));
    mt.key = ktyp;
    mt.elem = etyp;
    mt.bucket = bucketOf(ktyp, etyp);
    if (ktyp->size > maxKeySize) {
        mt.keysize = uint8_t(ptrSize);
        mt.indirectkey = 1;
    } else {
        mt.keysize = uint8_t(ktyp->size);
        mt.indirectkey = 0;
    }
    if (etyp->size > maxValSize) {
        mt.valuesize = uint8_t(ptrSize);
        mt.indirectvalue = 1;
    } else {
        mt.valuesize = uint8_t(etyp->size);
        mt.indirectvalue = 0;
    }
    mt.bucketsize = uint16_t(mt.bucket->size);
    mt.reflexivekey = isReflexive(ktyp);
    mt.needkeyupdate = needKeyUpdate(ktyp);
    mt.ptrToThis = 0;
    std::tie(ti, _) = lookupCache.LoadOrStore(
            moku::make_iface<cacheKey>(ckey),
            moku::make_iface<rtype *>(&{
                    % !s(token.Pos = 432383) &
                    % !s(*ast.SelectorExpr = &{0xc420dab2a0 0xc420dab2c0})}));
    return moku::try_assert<Type>(ti);
}
Type FuncOf(moku::slice<Type> in, moku::slice<Type> out, bool variadic)
{
    moku::defer _defer_;
    std::function<Type(rtype *)> addToCache{std::nullptr};
    moku::slice<rtype *> args{};
    funcType *ft{std::nullptr};
    uint32_t hash{0};
    moku::interface ifunc{};
    int n{0};
    funcType *prototype{std::nullptr};
    std::string str{""};

    if (variadic && (len(in) == 0 || in[len(in) - 1].Kind() != Slice)) {
        panic(moku::make_iface<std::string>(&{
                % !s(token.Pos = 433259) STRING
                  "reflect.FuncOf: last arg of variadic func must be slice"}));
    }
    prototype = *(**funcType)(unsafe::Pointer(&ifunc));
    n = len(in) + len(out);
    if ((n <= 4)) {
    _ident_28_:
        fixed = new (funcTypeFixed4);
        args = moku::slice_expr<std::vector<rtype *>>(
                fixed->args, 0, len(fixed->args));
        ft = &fixed->funcType;
    } else if ((n <= 8)) {
    _ident_29_:
        fixed = new (funcTypeFixed8);
        args = moku::slice_expr<std::vector<rtype *>>(
                fixed->args, 0, len(fixed->args));
        ft = &fixed->funcType;
    } else if ((n <= 16)) {
    _ident_30_:
        fixed = new (funcTypeFixed16);
        args = moku::slice_expr<std::vector<rtype *>>(
                fixed->args, 0, len(fixed->args));
        ft = &fixed->funcType;
    } else if ((n <= 32)) {
    _ident_31_:
        fixed = new (funcTypeFixed32);
        args = moku::slice_expr<std::vector<rtype *>>(
                fixed->args, 0, len(fixed->args));
        ft = &fixed->funcType;
    } else if ((n <= 64)) {
    _ident_32_:
        fixed = new (funcTypeFixed64);
        args = moku::slice_expr<std::vector<rtype *>>(
                fixed->args, 0, len(fixed->args));
        ft = &fixed->funcType;
    } else if ((n <= 128)) {
    _ident_33_:
        fixed = new (funcTypeFixed128);
        args = moku::slice_expr<std::vector<rtype *>>(
                fixed->args, 0, len(fixed->args));
        ft = &fixed->funcType;
    } else {
    _ident_34_:
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 434184) STRING
                     "reflect.FuncOf: too many arguments"}));
    }
    *ft = *prototype;
    {
        Type in{};
        for (in : moku::range_value<moku::slice<Type>>(in)) {
            t = moku::type_assert<*rtype>(in);
            args = append(args, t);
            hash = fnv1(hash,
                        byte(t->hash >> 24),
                        byte(t->hash >> 16),
                        byte(t->hash >> 8),
                        byte(t->hash));
        }
    }
    if (variadic) {
        hash = fnv1(hash, 'v');
    }
    hash = fnv1(hash, '.');
    {
        Type out{};
        for (out : moku::range_value<moku::slice<Type>>(out)) {
            t = moku::type_assert<*rtype>(out);
            args = append(args, t);
            hash = fnv1(hash,
                        byte(t->hash >> 24),
                        byte(t->hash >> 16),
                        byte(t->hash >> 8),
                        byte(t->hash));
        }
    }
    if (len(args) > 50) {
        panic(moku::make_iface<std::string>(&{
                % !s(token.Pos = 434722) STRING
                  "reflect.FuncOf does not support more than 50 arguments"}));
    }
    ft->tflag = 0;
    ft->hash = hash;
    ft->inCount = uint16_t(len(in));
    ft->outCount = uint16_t(len(out));
    if (variadic) {
        ft->outCount |= 1 << 15;
    }
    {
        bool ok{false};
        moku::interface ts{};

        std::tie(ts, ok) =
                funcLookupCache.m.Load(moku::make_iface<uint32_t>(hash));

        if (ok) {
            {
                rtype *t{std::nullptr};
                for (t : moku::range_value<moku::slice<rtype *>>(
                             moku::try_assert<moku::slice<*rtype>>(ts))) {
                    if (haveIdenticalUnderlyingType(&ft->rtype, t, true)) {
                        return t;
                    }
                }
            }
        }
    }
    funcLookupCache.Lock();
    _defer_.Push([=]() mutable { funcLookupCache.Unlock(); });
    {
        bool ok{false};
        moku::interface ts{};

        std::tie(ts, ok) =
                funcLookupCache.m.Load(moku::make_iface<uint32_t>(hash));

        if (ok) {
            {
                rtype *t{std::nullptr};
                for (t : moku::range_value<moku::slice<rtype *>>(
                             moku::try_assert<moku::slice<*rtype>>(ts))) {
                    if (haveIdenticalUnderlyingType(&ft->rtype, t, true)) {
                        return t;
                    }
                }
            }
        }
    }
    addToCache = [=](rtype *tt) mutable -> Type {
        {
            bool ok{false};
            moku::interface rti{};

            std::tie(rti, ok) =
                    funcLookupCache.m.Load(moku::make_iface<uint32_t>(hash));

            if (ok) {
                rts = moku::type_assert<moku::slice<*rtype>>(rti);
            }
        }
        funcLookupCache.m.Store(
                moku::make_iface<uint32_t>(hash),
                moku::make_iface<moku::slice<rtype *>>(
                        &{append % !s(token.Pos = 435536)[rts tt] %
                          !s(token.Pos = 0) % !s(token.Pos = 435544)}));
        return tt;
    };
    str = funcStr(ft);
    {
        rtype *tt{std::nullptr};
        for (tt : moku::range_value<moku::slice<rtype *>>(typesByString(str))) {
            if (haveIdenticalUnderlyingType(&ft->rtype, tt, true)) {
                return addToCache(tt);
            }
        }
    }
    ft->str = resolveReflectName(newName(str, "", "", false));
    ft->ptrToThis = 0;
    return addToCache(&ft->rtype);
}
std::string funcStr(funcType *ft)
{
    moku::slice<rtype *> out{};
    moku::slice<uint8_t> repr{};

    repr = make(moku::slice<byte>, 0, 64);
    repr = append(repr, "func(");
    {
        int i{0};
        rtype *t{std::nullptr};
        for (std::tie(i, t) :
             moku::range_key_value<moku::slice<rtype *>>(ft->in())) {
            if (i > 0) {
                repr = append(repr, ", ");
            }
            if (ft->IsVariadic() && i == int(ft->inCount) - 1) {
                repr = append(repr, "...");
                repr = append(repr,
                              (*sliceType)(unsafe::Pointer(t))->elem->String());
            } else {
                repr = append(repr, t->String());
            }
        }
    }
    repr = append(repr, ')');
    out = ft->out();
    if (len(out) == 1) {
        repr = append(repr, ' ');
    } else {
        if (len(out) > 1) {
            repr = append(repr, " (");
        }
    }
    {
        int i{0};
        rtype *t{std::nullptr};
        for (std::tie(i, t) :
             moku::range_key_value<moku::slice<rtype *>>(out)) {
            if (i > 0) {
                repr = append(repr, ", ");
            }
            repr = append(repr, t->String());
        }
    }
    if (len(out) > 1) {
        repr = append(repr, ')');
    }
    return std::string(repr);
}
bool isReflexive(rtype *t)
{
    if ((t->Kind() == Bool) || (t->Kind() == Int) || (t->Kind() == Int8) ||
        (t->Kind() == Int16) || (t->Kind() == Int32) || (t->Kind() == Int64) ||
        (t->Kind() == Uint) || (t->Kind() == Uint8) || (t->Kind() == Uint16) ||
        (t->Kind() == Uint32) || (t->Kind() == Uint64) ||
        (t->Kind() == Uintptr) || (t->Kind() == Chan) || (t->Kind() == Ptr) ||
        (t->Kind() == String) || (t->Kind() == UnsafePointer)) {
    _ident_35_:
        return true;
    } else if ((t->Kind() == Float32) || (t->Kind() == Float64) ||
               (t->Kind() == Complex64) || (t->Kind() == Complex128) ||
               (t->Kind() == Interface)) {
    _ident_36_:
        return false;
    } else if ((t->Kind() == Array)) {
    _ident_37_:
        tt = (*arrayType)(unsafe::Pointer(t));
        return isReflexive(tt->elem);
    } else if ((t->Kind() == Struct)) {
    _ident_38_:
        tt = (*structType)(unsafe::Pointer(t));
        {
            structField f{};
            for (f : moku::range_value<moku::slice<structField>>(tt->fields)) {
                if (!isReflexive(f.typ)) {
                    return false;
                }
            }
        }
        return true;
    } else {
    _ident_39_:
        panic(moku::make_iface<std::string>(
                &{ %
                           !s(*ast.BasicLit = &{437395 9 "isReflexive called "
                                                         "on non-key type "}) %
                           !s(token.Pos = 437433) +
                   % !s(*ast.CallExpr = &{0xc420e50940 437443 [] 0 437444})}));
    }
}
bool needKeyUpdate(rtype *t)
{
    if ((t->Kind() == Bool) || (t->Kind() == Int) || (t->Kind() == Int8) ||
        (t->Kind() == Int16) || (t->Kind() == Int32) || (t->Kind() == Int64) ||
        (t->Kind() == Uint) || (t->Kind() == Uint8) || (t->Kind() == Uint16) ||
        (t->Kind() == Uint32) || (t->Kind() == Uint64) ||
        (t->Kind() == Uintptr) || (t->Kind() == Chan) || (t->Kind() == Ptr) ||
        (t->Kind() == UnsafePointer)) {
    _ident_40_:
        return false;
    } else if ((t->Kind() == Float32) || (t->Kind() == Float64) ||
               (t->Kind() == Complex64) || (t->Kind() == Complex128) ||
               (t->Kind() == Interface) || (t->Kind() == String)) {
    _ident_41_:
        return true;
    } else if ((t->Kind() == Array)) {
    _ident_42_:
        tt = (*arrayType)(unsafe::Pointer(t));
        return needKeyUpdate(tt->elem);
    } else if ((t->Kind() == Struct)) {
    _ident_43_:
        tt = (*structType)(unsafe::Pointer(t));
        {
            structField f{};
            for (f : moku::range_value<moku::slice<structField>>(tt->fields)) {
                if (needKeyUpdate(f.typ)) {
                    return true;
                }
            }
        }
        return false;
    } else {
    _ident_44_:
        panic(moku::make_iface<std::string>(
                &{ %
                           !s(*ast.BasicLit = &{438251 9 "needKeyUpdate called "
                                                         "on non-key type "}) %
                           !s(token.Pos = 438291) +
                   % !s(*ast.CallExpr = &{0xc420e51380 438301 [] 0 438302})}));
    }
}
rtype *bucketOf(rtype *ktyp, rtype *etyp)
{
    rtype *b{std::nullptr};
    uint8_t *gcdata{std::nullptr};
    uint8_t kind{0};
    uintptr_t overflowPad{0};
    uintptr_t ptrdata{0};
    std::string s{""};
    uintptr_t size{0};

    if (ktyp->kind & kindNoPointers != 0 && etyp->kind & kindNoPointers != 0 &&
        ktyp->size <= maxKeySize && etyp->size <= maxValSize) {
        kind = kindNoPointers;
    }
    if (ktyp->size > maxKeySize) {
        ktyp = moku::type_assert<*rtype>(PtrTo(ktyp));
    }
    if (etyp->size > maxValSize) {
        etyp = moku::type_assert<*rtype>(PtrTo(etyp));
    }
    if (runtime::GOARCH == "amd64p32" &&
        (ktyp->align > ptrSize || etyp->align > ptrSize)) {
        overflowPad = ptrSize;
    }
    size = bucketSize * (1 + ktyp->size + etyp->size) + overflowPad + ptrSize;
    if (size & uintptr_t(ktyp->align - 1) != 0 ||
        size & uintptr_t(etyp->align - 1) != 0) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 439817) STRING
                     "reflect: bad size computation in MapOf"}));
    }
    if (kind != kindNoPointers) {
        nptr = (bucketSize * (1 + ktyp->size + etyp->size) + ptrSize) / ptrSize;
        mask = make(moku::slice<byte>, (nptr + 7) / 8);
        base = bucketSize / ptrSize;
        if (ktyp->kind & kindNoPointers == 0) {
            if (ktyp->kind & kindGCProg != 0) {
                panic(moku::make_iface<std::string>(
                        &{ % !s(token.Pos = 440107) STRING
                             "reflect: unexpected GC program in MapOf"}));
            }
            kmask = (*std::vector<byte>)(unsafe::Pointer(ktyp->gcdata));
            {
                uintptr_t i{0};
                for (i = uintptr_t(0); i < ktyp->ptrdata / ptrSize; i++) {
                    if ((kmask[i / 8] >> (i % 8)) & 1 != 0) {
                        {
                            uintptr_t j{0};
                            for (j = uintptr_t(0); j < bucketSize; j++) {
                                uintptr_t word{0};

                                word = base + j * ktyp->size / ptrSize + i;
                                mask[word / 8] |= 1 << (word % 8);
                            }
                        }
                    }
                }
            }
        }
        base += bucketSize * ktyp->size / ptrSize;
        if (etyp->kind & kindNoPointers == 0) {
            if (etyp->kind & kindGCProg != 0) {
                panic(moku::make_iface<std::string>(
                        &{ % !s(token.Pos = 440578) STRING
                             "reflect: unexpected GC program in MapOf"}));
            }
            emask = (*std::vector<byte>)(unsafe::Pointer(etyp->gcdata));
            {
                uintptr_t i{0};
                for (i = uintptr_t(0); i < etyp->ptrdata / ptrSize; i++) {
                    if ((emask[i / 8] >> (i % 8)) & 1 != 0) {
                        {
                            uintptr_t j{0};
                            for (j = uintptr_t(0); j < bucketSize; j++) {
                                uintptr_t word{0};

                                word = base + j * etyp->size / ptrSize + i;
                                mask[word / 8] |= 1 << (word % 8);
                            }
                        }
                    }
                }
            }
        }
        base += bucketSize * etyp->size / ptrSize;
        base += overflowPad / ptrSize;
        word = base;
        mask[word / 8] |= 1 << (word % 8);
        gcdata = &mask[0];
        ptrdata = (word + 1) * ptrSize;
        if (ptrdata != size) {
            panic(moku::make_iface<std::string>(
                    &{ % !s(token.Pos = 441167) STRING
                         "reflect: bad layout computation in MapOf"}));
        }
    }
    b = &rtype{{align, ptrSize},
               {size, size},
               {kind, kind},
               {ptrdata, ptrdata},
               {gcdata, gcdata}};
    if (overflowPad > 0) {
        b->align = 8;
    }
    s = "bucket(" + ktyp->String() + "," + etyp->String() + ")";
    b->str = resolveReflectName(newName(s, "", "", false));
    return b;
}
Type SliceOf(Type t)
{
    cacheKey ckey{};
    moku::interface islice{};
    sliceType *prototype{std::nullptr};
    std::string s{""};
    sliceType slice{};
    moku::interface ti{};
    rtype *typ{std::nullptr};

    typ = moku::type_assert<*rtype>(t);
    ckey = cacheKey{Slice, typ, nil, 0};
    {
        bool ok{false};
        moku::interface slice{};

        std::tie(slice, ok) =
                lookupCache.Load(moku::make_iface<cacheKey>(ckey));

        if (ok) {
            return moku::try_assert<Type>(slice);
        }
    }
    s = "[]" + typ->String();
    {
        rtype *tt{std::nullptr};
        for (tt : moku::range_value<moku::slice<rtype *>>(typesByString(s))) {
            slice = (*sliceType)(unsafe::Pointer(tt));
            if (slice->elem == typ) {
                std::tie(ti, _) = lookupCache.LoadOrStore(
                        moku::make_iface<cacheKey>(ckey),
                        moku::make_iface<rtype *>(tt));
                return moku::try_assert<Type>(ti);
            }
        }
    }
    prototype = *(**sliceType)(unsafe::Pointer(&islice));
    slice = *prototype;
    slice.tflag = 0;
    slice.str = resolveReflectName(newName(s, "", "", false));
    slice.hash = fnv1(typ->hash, '[');
    slice.elem = typ;
    slice.ptrToThis = 0;
    std::tie(ti, _) = lookupCache.LoadOrStore(
            moku::make_iface<cacheKey>(ckey),
            moku::make_iface<rtype *>(&{
                    % !s(token.Pos = 442365) &
                    % !s(*ast.SelectorExpr = &{0xc420eb3880 0xc420eb38a0})}));
    return moku::try_assert<Type>(ti);
}
bool isLetter(int32_t ch)
{
    return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_' ||
           ch >= utf8::RuneSelf && unicode::IsLetter(ch);
}
bool isValidFieldName(std::string fieldName)
{
    {
        int32_t c{0};
        int i{0};
        for (std::tie(i, c) : moku::range_key_value<std::string>(fieldName)) {
            if (i == 0 && !isLetter(c)) {
                return false;
            }
            if (!(isLetter(c) || unicode::IsDigit(c))) {
                return false;
            }
        }
    }
    return len(fieldName) > 0;
}
Type StructOf(moku::slice<StructField> fields)
{
}
