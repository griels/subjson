#pragma once
#include "math.h"
#include "runtime.h"
#include "strconv.h"
#include "sync.h"
#include "unicode.h"
#include "unsafe.h"
#include "utf8.h"
namespace reflect {
Value Append(Value s, moku::slice<Value> x);
Value AppendSlice(Value s, Value t);
constexpr Kind Array{17};
Type ArrayOf(int count, Type elem);
constexpr Kind Bool{1};
constexpr ChanDir BothDir{3};
constexpr Kind Chan{18};

struct ChanDir : public moku::basic<int>, public Type {
    ChanDir() : moku::basic<int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<ChanDir>(const ChanDir &chandir)
{
    return moku::is_nil<int>(int(chandir));
}
template <>
ChanDir *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
inline std::string String()
{
    ChanDir chandir = *this;
    return chandir._StringByValue();
}
std::string _StringByValue();
Type ChanOf(ChanDir dir, Type t);
constexpr Kind Complex128{16};
constexpr Kind Complex64{15};
int Copy(Value dst, Value src);
bool DeepEqual(moku::interface x, moku::interface y);
constexpr Kind Float32{13};
constexpr Kind Float64{14};
constexpr Kind Func{19};
Type FuncOf(moku::slice<Type> in, moku::slice<Type> out, bool variadic);
Value Indirect(Value v);
constexpr Kind Int{2};
constexpr Kind Int16{4};
constexpr Kind Int32{5};
constexpr Kind Int64{6};
constexpr Kind Int8{3};
constexpr Kind Interface{20};
constexpr Kind Invalid{0};

struct Kind : public moku::basic<unsigned int>, public Type {
    Kind() : moku::basic<unsigned int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<Kind>(const Kind &kind)
{
    return moku::is_nil<unsigned int>(unsigned int(kind));
}
template <>
Kind *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
inline std::string String()
{
    Kind kind = *this;
    return kind._StringByValue();
}
std::string _StringByValue();
Value MakeChan(Type typ, int buffer);
Value MakeFunc(Type typ,
               std::function<moku::slice<Value>(moku::slice<Value>)> fn);
Value MakeMap(Type typ);
Value MakeMapWithSize(Type typ, int n);
Value MakeSlice(Type typ, int len, int cap);
constexpr Kind Map{21};
Type MapOf(Type key, Type elem);

struct Method : public Type {
    std::string Name{""};
    std::string PkgPath{""};
    Type Type;
    Value Func;
    int Index{0};
};
template <>
inline bool moku::is_nil<Method>(const Method &method)
{
    return moku::is_nil<std::string>(method.Name) &&
           moku::is_nil<std::string>(method.PkgPath) &&
           moku::is_nil<Type>(method.Type) &&
           moku::is_nil<Value>(method.Func) && moku::is_nil<int>(method.Index);
}
template <>
Method *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
Value New(Type typ);
Value NewAt(Type typ, void *p);
constexpr Kind Ptr{22};
Type PtrTo(Type t);
constexpr ChanDir RecvDir{1};
std::tuple<int, Value, bool> Select(moku::slice<SelectCase> cases);

struct SelectCase : public Type {
    SelectDir Dir;
    Value Chan;
    Value Send;
};
template <>
inline bool moku::is_nil<SelectCase>(const SelectCase &selectcase)
{
    return moku::is_nil<SelectDir>(selectcase.Dir) &&
           moku::is_nil<Value>(selectcase.Chan) &&
           moku::is_nil<Value>(selectcase.Send);
}
template <>
SelectCase *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
constexpr SelectDir SelectDefault{3};

struct SelectDir : public moku::basic<int>, public Type {
    SelectDir() : moku::basic<int>{0}
    {
    }
};
template <>
inline bool moku::is_nil<SelectDir>(const SelectDir &selectdir)
{
    return moku::is_nil<int>(int(selectdir));
}
template <>
SelectDir *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
constexpr SelectDir SelectRecv{2};
constexpr SelectDir SelectSend{1};
constexpr ChanDir SendDir{2};
constexpr Kind Slice{23};

struct SliceHeader : public Type {
    uintptr_t Data{0};
    int Len{0};
    int Cap{0};
};
template <>
inline bool moku::is_nil<SliceHeader>(const SliceHeader &sliceheader)
{
    return moku::is_nil<uintptr_t>(sliceheader.Data) &&
           moku::is_nil<int>(sliceheader.Len) &&
           moku::is_nil<int>(sliceheader.Cap);
}
template <>
SliceHeader *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
Type SliceOf(Type t);
constexpr Kind String{24};

struct StringHeader : public Type {
    uintptr_t Data{0};
    int Len{0};
};
template <>
inline bool moku::is_nil<StringHeader>(const StringHeader &stringheader)
{
    return moku::is_nil<uintptr_t>(stringheader.Data) &&
           moku::is_nil<int>(stringheader.Len);
}
template <>
StringHeader *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
constexpr Kind Struct{25};

struct StructField : public Type {
    std::string Name{""};
    std::string PkgPath{""};
    Type Type;
    StructTag Tag;
    uintptr_t Offset{0};
    moku::slice<int> Index;
    bool Anonymous{false};
};
template <>
inline bool moku::is_nil<StructField>(const StructField &structfield)
{
    return moku::is_nil<std::string>(structfield.Name) &&
           moku::is_nil<std::string>(structfield.PkgPath) &&
           moku::is_nil<Type>(structfield.Type) &&
           moku::is_nil<StructTag>(structfield.Tag) &&
           moku::is_nil<uintptr_t>(structfield.Offset) &&
           moku::is_nil<moku::slice<int>>(structfield.Index) &&
           moku::is_nil<bool>(structfield.Anonymous);
}
template <>
StructField *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
Type StructOf(moku::slice<StructField> fields);

struct StructTag : public moku::basic<std::string>, public Type {
    StructTag() : moku::basic<std::string>{""}
    {
    }
};
template <>
inline bool moku::is_nil<StructTag>(const StructTag &structtag)
{
    return moku::is_nil<std::string>(std::string(structtag));
}
template <>
StructTag *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
inline std::string Get(std::string key)
{
    StructTag structtag = *this;
    return structtag._GetByValue(std::string key);
}
std::string _GetByValue(std::string key);
inline std::tuple<std::string, bool> Lookup(std::string key)
{
    StructTag structtag = *this;
    return structtag._LookupByValue(std::string key);
}
std::tuple<std::string, bool> _LookupByValue(std::string key);
std::function<void(int, int)> Swapper(moku::interface slice);

struct Type {
    virtual uncommonType *uncommon() = 0;
    virtual rtype *common() = 0;
    virtual std::string String() = 0;
    virtual uintptr_t Size() = 0;
    virtual std::string PkgPath() = 0;
    virtual Type Out(int i) = 0;
    virtual int NumOut() = 0;
    virtual int NumMethod() = 0;
    virtual int NumIn() = 0;
    virtual int NumField() = 0;
    virtual std::string Name() = 0;
    virtual std::tuple<Method, bool> MethodByName(std::string) = 0;
    virtual Method Method(int) = 0;
    virtual int Len() = 0;
    virtual Kind Kind() = 0;
    virtual Type Key() = 0;
    virtual bool IsVariadic() = 0;
    virtual Type In(int i) = 0;
    virtual bool Implements(Type u) = 0;
    virtual std::tuple<StructField, bool> FieldByNameFunc(
            std::function<bool(std::string)> match) = 0;
    virtual std::tuple<StructField, bool> FieldByName(std::string name) = 0;
    virtual StructField FieldByIndex(moku::slice<int> index) = 0;
    virtual int FieldAlign() = 0;
    virtual StructField Field(int i) = 0;
    virtual Type Elem() = 0;
    virtual bool ConvertibleTo(Type u) = 0;
    virtual bool Comparable() = 0;
    virtual ChanDir ChanDir() = 0;
    virtual int Bits() = 0;
    virtual bool AssignableTo(Type u) = 0;
    virtual int Align() = 0;
};
template <>
inline visit *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<visit>(iface);
}
template <>
inline structTypeFixed4 *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<structTypeFixed4>(iface);
}
template <>
inline sliceHeader *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<sliceHeader>(iface);
}
template <>
inline SliceHeader *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<SliceHeader>(iface);
}
template <>
inline ValueError *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<ValueError>(iface);
}
template <>
inline structTypeFixed16 *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<structTypeFixed16>(iface);
}
template <>
inline StructField *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<StructField>(iface);
}
template <>
inline ptrType *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<ptrType>(iface);
}
template <>
inline mapType *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<mapType>(iface);
}
template <>
inline runtimeSelect *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<runtimeSelect>(iface);
}
template <>
inline bitVector *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<bitVector>(iface);
}
template <>
inline structTypeUncommon *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<structTypeUncommon>(iface);
}
template <>
inline u *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<u>(iface);
}
template <>
inline structTypeFixed8 *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<structTypeFixed8>(iface);
}
template <>
inline layoutKey *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<layoutKey>(iface);
}
template <>
inline structField *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<structField>(iface);
}
template <>
inline funcType *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<funcType>(iface);
}
template <>
inline funcTypeFixed4 *moku::try_assert(const moku::interface &iface)
{
    return moku::type_registry::try_assert<funcTypeFixed4>(iface);
}
Type TypeOf(moku::interface i);
constexpr Kind Uint{7};
constexpr Kind Uint16{9};
constexpr Kind Uint32{10};
constexpr Kind Uint64{11};
constexpr Kind Uint8{8};
constexpr Kind Uintptr{12};
constexpr Kind UnsafePointer{26};

struct Value : public Type {
    rtype *typ{std::nullptr};
    void *ptr{std::nullptr};
    flag flag;
    inline void *pointer()
    {
        Value value = *this;
        return value._pointerByValue();
    }
    void *_pointerByValue();
    inline Value Addr()
    {
        Value value = *this;
        return value._AddrByValue();
    }
    Value _AddrByValue();
    inline bool Bool()
    {
        Value value = *this;
        return value._BoolByValue();
    }
    bool _BoolByValue();
    inline moku::slice<uint8_t> Bytes()
    {
        Value value = *this;
        return value._BytesByValue();
    }
    moku::slice<uint8_t> _BytesByValue();
    inline moku::slice<int32_t> runes()
    {
        Value value = *this;
        return value._runesByValue();
    }
    moku::slice<int32_t> _runesByValue();
    inline bool CanAddr()
    {
        Value value = *this;
        return value._CanAddrByValue();
    }
    bool _CanAddrByValue();
    inline bool CanSet()
    {
        Value value = *this;
        return value._CanSetByValue();
    }
    bool _CanSetByValue();
    inline moku::slice<Value> Call(moku::slice<Value> in)
    {
        Value value = *this;
        return value._CallByValue(moku::slice<Value> in);
    }
    moku::slice<Value> _CallByValue(moku::slice<Value> in);
    inline moku::slice<Value> CallSlice(moku::slice<Value> in)
    {
        Value value = *this;
        return value._CallSliceByValue(moku::slice<Value> in);
    }
    moku::slice<Value> _CallSliceByValue(moku::slice<Value> in);
    inline moku::slice<Value> call(std::string op, moku::slice<Value> in)
    {
        Value value = *this;
        return value._callByValue(std::string op, moku::slice<Value> in);
    }
    moku::slice<Value> _callByValue(std::string op, moku::slice<Value> in);
    inline int Cap()
    {
        Value value = *this;
        return value._CapByValue();
    }
    int _CapByValue();
    inline void Close()
    {
        Value value = *this;
        value._CloseByValue();
    }
    void _CloseByValue();
    inline moku::complex<double> Complex()
    {
        Value value = *this;
        return value._ComplexByValue();
    }
    moku::complex<double> _ComplexByValue();
    inline Value Elem()
    {
        Value value = *this;
        return value._ElemByValue();
    }
    Value _ElemByValue();
    inline Value Field(int i)
    {
        Value value = *this;
        return value._FieldByValue(int i);
    }
    Value _FieldByValue(int i);
    inline Value FieldByIndex(moku::slice<int> index)
    {
        Value value = *this;
        return value._FieldByIndexByValue(moku::slice<int> index);
    }
    Value _FieldByIndexByValue(moku::slice<int> index);
    inline Value FieldByName(std::string name)
    {
        Value value = *this;
        return value._FieldByNameByValue(std::string name);
    }
    Value _FieldByNameByValue(std::string name);
    inline Value FieldByNameFunc(std::function<bool(std::string)> match)
    {
        Value value = *this;
        return value._FieldByNameFuncByValue(
                std::function<bool(std::string)> match);
    }
    Value _FieldByNameFuncByValue(std::function<bool(std::string)> match);
    inline double Float()
    {
        Value value = *this;
        return value._FloatByValue();
    }
    double _FloatByValue();
    inline Value Index(int i)
    {
        Value value = *this;
        return value._IndexByValue(int i);
    }
    Value _IndexByValue(int i);
    inline int64_t Int()
    {
        Value value = *this;
        return value._IntByValue();
    }
    int64_t _IntByValue();
    inline bool CanInterface()
    {
        Value value = *this;
        return value._CanInterfaceByValue();
    }
    bool _CanInterfaceByValue();
    inline moku::interface Interface()
    {
        Value value = *this;
        return value._InterfaceByValue();
    }
    moku::interface _InterfaceByValue();
    inline std::vector<uintptr_t> InterfaceData()
    {
        Value value = *this;
        return value._InterfaceDataByValue();
    }
    std::vector<uintptr_t> _InterfaceDataByValue();
    inline bool IsNil()
    {
        Value value = *this;
        return value._IsNilByValue();
    }
    bool _IsNilByValue();
    inline bool IsValid()
    {
        Value value = *this;
        return value._IsValidByValue();
    }
    bool _IsValidByValue();
    inline Kind Kind()
    {
        Value value = *this;
        return value._KindByValue();
    }
    Kind _KindByValue();
    inline int Len()
    {
        Value value = *this;
        return value._LenByValue();
    }
    int _LenByValue();
    inline Value MapIndex(Value key)
    {
        Value value = *this;
        return value._MapIndexByValue(Value key);
    }
    Value _MapIndexByValue(Value key);
    inline moku::slice<Value> MapKeys()
    {
        Value value = *this;
        return value._MapKeysByValue();
    }
    moku::slice<Value> _MapKeysByValue();
    inline Value Method(int i)
    {
        Value value = *this;
        return value._MethodByValue(int i);
    }
    Value _MethodByValue(int i);
    inline int NumMethod()
    {
        Value value = *this;
        return value._NumMethodByValue();
    }
    int _NumMethodByValue();
    inline Value MethodByName(std::string name)
    {
        Value value = *this;
        return value._MethodByNameByValue(std::string name);
    }
    Value _MethodByNameByValue(std::string name);
    inline int NumField()
    {
        Value value = *this;
        return value._NumFieldByValue();
    }
    int _NumFieldByValue();
    inline bool OverflowComplex(moku::complex<double> x)
    {
        Value value = *this;
        return value._OverflowComplexByValue(moku::complex<double> x);
    }
    bool _OverflowComplexByValue(moku::complex<double> x);
    inline bool OverflowFloat(double x)
    {
        Value value = *this;
        return value._OverflowFloatByValue(double x);
    }
    bool _OverflowFloatByValue(double x);
    inline bool OverflowInt(int64_t x)
    {
        Value value = *this;
        return value._OverflowIntByValue(int64_t x);
    }
    bool _OverflowIntByValue(int64_t x);
    inline bool OverflowUint(uint64_t x)
    {
        Value value = *this;
        return value._OverflowUintByValue(uint64_t x);
    }
    bool _OverflowUintByValue(uint64_t x);
    inline uintptr_t Pointer()
    {
        Value value = *this;
        return value._PointerByValue();
    }
    uintptr_t _PointerByValue();
    inline std::tuple<Value, bool> Recv()
    {
        Value value = *this;
        return value._RecvByValue();
    }
    std::tuple<Value, bool> _RecvByValue();
    inline std::tuple<Value, bool> recv(bool nb)
    {
        Value value = *this;
        return value._recvByValue(bool nb);
    }
    std::tuple<Value, bool> _recvByValue(bool nb);
    inline void Send(Value x)
    {
        Value value = *this;
        value._SendByValue(Value x);
    }
    void _SendByValue(Value x);
    inline bool send(Value x, bool nb)
    {
        Value value = *this;
        return value._sendByValue(Value x, bool nb);
    }
    bool _sendByValue(Value x, bool nb);
    inline void Set(Value x)
    {
        Value value = *this;
        value._SetByValue(Value x);
    }
    void _SetByValue(Value x);
    inline void SetBool(bool x)
    {
        Value value = *this;
        value._SetBoolByValue(bool x);
    }
    void _SetBoolByValue(bool x);
    inline void SetBytes(moku::slice<uint8_t> x)
    {
        Value value = *this;
        value._SetBytesByValue(moku::slice<uint8_t> x);
    }
    void _SetBytesByValue(moku::slice<uint8_t> x);
    inline void setRunes(moku::slice<int32_t> x)
    {
        Value value = *this;
        value._setRunesByValue(moku::slice<int32_t> x);
    }
    void _setRunesByValue(moku::slice<int32_t> x);
    inline void SetComplex(moku::complex<double> x)
    {
        Value value = *this;
        value._SetComplexByValue(moku::complex<double> x);
    }
    void _SetComplexByValue(moku::complex<double> x);
    inline void SetFloat(double x)
    {
        Value value = *this;
        value._SetFloatByValue(double x);
    }
    void _SetFloatByValue(double x);
    inline void SetInt(int64_t x)
    {
        Value value = *this;
        value._SetIntByValue(int64_t x);
    }
    void _SetIntByValue(int64_t x);
    inline void SetLen(int n)
    {
        Value value = *this;
        value._SetLenByValue(int n);
    }
    void _SetLenByValue(int n);
    inline void SetCap(int n)
    {
        Value value = *this;
        value._SetCapByValue(int n);
    }
    void _SetCapByValue(int n);
    inline void SetMapIndex(Value key, Value val)
    {
        Value value = *this;
        value._SetMapIndexByValue(Value key, Value val);
    }
    void _SetMapIndexByValue(Value key, Value val);
    inline void SetUint(uint64_t x)
    {
        Value value = *this;
        value._SetUintByValue(uint64_t x);
    }
    void _SetUintByValue(uint64_t x);
    inline void SetPointer(void *x)
    {
        Value value = *this;
        value._SetPointerByValue(void *x);
    }
    void _SetPointerByValue(void *x);
    inline void SetString(std::string x)
    {
        Value value = *this;
        value._SetStringByValue(std::string x);
    }
    void _SetStringByValue(std::string x);
    inline Value Slice(int i, int j)
    {
        Value value = *this;
        return value._SliceByValue(int i, int j);
    }
    Value _SliceByValue(int i, int j);
    inline Value Slice3(int i, int j, int k)
    {
        Value value = *this;
        return value._Slice3ByValue(int i, int j, int k);
    }
    Value _Slice3ByValue(int i, int j, int k);
    inline std::string String()
    {
        Value value = *this;
        return value._StringByValue();
    }
    std::string _StringByValue();
    inline std::tuple<Value, bool> TryRecv()
    {
        Value value = *this;
        return value._TryRecvByValue();
    }
    std::tuple<Value, bool> _TryRecvByValue();
    inline bool TrySend(Value x)
    {
        Value value = *this;
        return value._TrySendByValue(Value x);
    }
    bool _TrySendByValue(Value x);
    inline Type Type()
    {
        Value value = *this;
        return value._TypeByValue();
    }
    Type _TypeByValue();
    inline uint64_t Uint()
    {
        Value value = *this;
        return value._UintByValue();
    }
    uint64_t _UintByValue();
    inline uintptr_t UnsafeAddr()
    {
        Value value = *this;
        return value._UnsafeAddrByValue();
    }
    uintptr_t _UnsafeAddrByValue();
    inline Value assignTo(std::string context, rtype *dst, void *target)
    {
        Value value = *this;
        return value._assignToByValue(
                std::string context, rtype * dst, void *target);
    }
    Value _assignToByValue(std::string context, rtype *dst, void *target);
    inline Value Convert(Type t)
    {
        Value value = *this;
        return value._ConvertByValue(Type t);
    }
    Value _ConvertByValue(Type t);
};
template <>
inline bool moku::is_nil<Value>(const Value &value)
{
    return moku::is_nil<rtype *>(value.typ) &&
           moku::is_nil<void *>(value.ptr) && moku::is_nil<flag>(value.flag);
}
template <>
Value *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct ValueError : public Type {
    std::string Method{""};
    Kind Kind;
    std::string Error();
};
template <>
inline bool moku::is_nil<ValueError>(const ValueError &valueerror)
{
    return moku::is_nil<std::string>(valueerror.Method) &&
           moku::is_nil<Kind>(valueerror.Kind);
}
template <>
ValueError *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
Value ValueOf(moku::interface i);
Value Zero(Type typ);
void *add(void *p, uintptr_t x);
int32_t addReflectOff(void *ptr);
void addTypeBits(bitVector *bv, uintptr_t offset, rtype *t);
uintptr_t align(uintptr_t x, uintptr_t n);
moku::slice<uint8_t> appendVarint(moku::slice<uint8_t> x, uintptr_t v);
void *arrayAt(void *p, int i, uintptr_t eltSize);

struct arrayType : public Type {
    rtype rtype;
    rtype *elem{std::nullptr};
    rtype *slice{std::nullptr};
    uintptr_t len{0};
};
template <>
inline bool moku::is_nil<arrayType>(const arrayType &arraytype)
{
    return moku::is_nil<rtype>(arraytype.rtype) &&
           moku::is_nil<rtype *>(arraytype.elem) &&
           moku::is_nil<rtype *>(arraytype.slice) &&
           moku::is_nil<uintptr_t>(arraytype.len);
}
template <>
arrayType *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct bitVector : public Type {
    uint32_t n{0};
    moku::slice<uint8_t> data;
    void append(uint8_t bit);
};
template <>
inline bool moku::is_nil<bitVector>(const bitVector &bitvector)
{
    return moku::is_nil<uint32_t>(bitvector.n) &&
           moku::is_nil<moku::slice<uint8_t>>(bitvector.data);
}
template <>
bitVector *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
rtype *bucketOf(rtype *ktyp, rtype *etyp);
static constexpr uintptr_t bucketSize{8};

struct cacheKey : public Type {
    Kind kind;
    rtype *t1{std::nullptr};
    rtype *t2{std::nullptr};
    uintptr_t extra{0};
};
template <>
inline bool moku::is_nil<cacheKey>(const cacheKey &cachekey)
{
    return moku::is_nil<Kind>(cachekey.kind) &&
           moku::is_nil<rtype *>(cachekey.t1) &&
           moku::is_nil<rtype *>(cachekey.t2) &&
           moku::is_nil<uintptr_t>(cachekey.extra);
}
template <>
cacheKey *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
void call(rtype *argtype, void *fn, void *arg, uint32_t n, uint32_t retoffset);
static bool callGC;
void callMethod(methodValue *ctxt, void *frame);
void callReflect(makeFuncImpl *ctxt, void *frame);

struct chanType : public Type {
    rtype rtype;
    rtype *elem{std::nullptr};
    uintptr_t dir{0};
};
template <>
inline bool moku::is_nil<chanType>(const chanType &chantype)
{
    return moku::is_nil<rtype>(chantype.rtype) &&
           moku::is_nil<rtype *>(chantype.elem) &&
           moku::is_nil<uintptr_t>(chantype.dir);
}
template <>
chanType *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
int chancap(void *ch);
void chanclose(void *ch);
int chanlen(void *ch);
std::tuple<bool, bool> chanrecv(void *ch, bool nb, void *val);
bool chansend(void *ch, void *val, bool nb);
std::function<Value(Value, Type)> convertOp(rtype *dst, rtype *src);
Value cvtBytesString(Value v, Type t);
Value cvtComplex(Value v, Type t);
Value cvtDirect(Value v, Type typ);
Value cvtFloat(Value v, Type t);
Value cvtFloatInt(Value v, Type t);
Value cvtFloatUint(Value v, Type t);
Value cvtI2I(Value v, Type typ);
Value cvtInt(Value v, Type t);
Value cvtIntFloat(Value v, Type t);
Value cvtIntString(Value v, Type t);
Value cvtRunesString(Value v, Type t);
Value cvtStringBytes(Value v, Type t);
Value cvtStringRunes(Value v, Type t);
Value cvtT2I(Value v, Type typ);
Value cvtUint(Value v, Type t);
Value cvtUintFloat(Value v, Type t);
Value cvtUintString(Value v, Type t);
bool deepValueEqual(Value v1,
                    Value v2,
                    std::map<visit, bool> visited,
                    int depth);
bool directlyAssignable(rtype *T, rtype *V);
static struct {
    bool b{false};
    moku::interface x;
} dummy;

struct emptyInterface : public Type {
    rtype *typ{std::nullptr};
    void *word{std::nullptr};
};
template <>
inline bool moku::is_nil<emptyInterface>(const emptyInterface &emptyinterface)
{
    return moku::is_nil<rtype *>(emptyinterface.typ) &&
           moku::is_nil<void *>(emptyinterface.word);
}
template <>
emptyInterface *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
void escapes(moku::interface x);

struct fieldScan : public Type {
    structType *typ{std::nullptr};
    moku::slice<int> index;
};
template <>
inline bool moku::is_nil<fieldScan>(const fieldScan &fieldscan)
{
    return moku::is_nil<structType *>(fieldscan.typ) &&
           moku::is_nil<moku::slice<int>>(fieldscan.index);
}
template <>
fieldScan *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct flag : public moku::basic<uintptr_t>, public Type {
    flag() : moku::basic<uintptr_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<flag>(const flag &flag)
{
    return moku::is_nil<uintptr_t>(uintptr_t(flag));
}
template <>
flag *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
inline Kind kind()
{
    flag flag = *this;
    return flag._kindByValue();
}
Kind _kindByValue();
inline void mustBe(Kind expected)
{
    flag flag = *this;
    flag._mustBeByValue(Kind expected);
}
void _mustBeByValue(Kind expected);
inline void mustBeExported()
{
    flag flag = *this;
    flag._mustBeExportedByValue();
}
void _mustBeExportedByValue();
inline void mustBeAssignable()
{
    flag flag = *this;
    flag._mustBeAssignableByValue();
}
void _mustBeAssignableByValue();
static constexpr flag flagAddr{256};
static constexpr flag flagEmbedRO{64};
static constexpr flag flagIndir{128};
static constexpr flag flagKindMask{31};
static constexpr int flagKindWidth{5};
static constexpr flag flagMethod{512};
static constexpr int flagMethodShift{10};
static constexpr flag flagRO{96};
static constexpr flag flagStickyRO{32};
uint32_t fnv1(uint32_t x, moku::slice<uint8_t> list);
std::tuple<rtype *, uintptr_t, uintptr_t, bitVector *, Pool *> funcLayout(
        rtype *t, rtype *rcvr);
static struct {
    Mutex Mutex;
    Map m;
} funcLookupCache;
std::string funcName(std::function<moku::slice<Value>(moku::slice<Value>)> f);
std::string funcStr(funcType *ft);

struct funcType : public Type {
    rtype rtype;
    uint16_t inCount{0};
    uint16_t outCount{0};
    moku::slice<rtype *> in();
    moku::slice<rtype *> out();
};
template <>
inline bool moku::is_nil<funcType>(const funcType &functype)
{
    return moku::is_nil<rtype>(functype.rtype) &&
           moku::is_nil<uint16_t>(functype.inCount) &&
           moku::is_nil<uint16_t>(functype.outCount);
}
template <>
funcType *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct funcTypeFixed128 : public Type {
    funcType funcType;
    std::vector<rtype *> args;
};
template <>
inline bool moku::is_nil<funcTypeFixed128>(
        const funcTypeFixed128 &functypefixed128)
{
    return moku::is_nil<funcType>(functypefixed128.funcType) &&
           moku::is_nil<std::vector<rtype *>>(functypefixed128.args);
}
template <>
funcTypeFixed128 *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct funcTypeFixed16 : public Type {
    funcType funcType;
    std::vector<rtype *> args;
};
template <>
inline bool moku::is_nil<funcTypeFixed16>(
        const funcTypeFixed16 &functypefixed16)
{
    return moku::is_nil<funcType>(functypefixed16.funcType) &&
           moku::is_nil<std::vector<rtype *>>(functypefixed16.args);
}
template <>
funcTypeFixed16 *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct funcTypeFixed32 : public Type {
    funcType funcType;
    std::vector<rtype *> args;
};
template <>
inline bool moku::is_nil<funcTypeFixed32>(
        const funcTypeFixed32 &functypefixed32)
{
    return moku::is_nil<funcType>(functypefixed32.funcType) &&
           moku::is_nil<std::vector<rtype *>>(functypefixed32.args);
}
template <>
funcTypeFixed32 *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct funcTypeFixed4 : public Type {
    funcType funcType;
    std::vector<rtype *> args;
};
template <>
inline bool moku::is_nil<funcTypeFixed4>(const funcTypeFixed4 &functypefixed4)
{
    return moku::is_nil<funcType>(functypefixed4.funcType) &&
           moku::is_nil<std::vector<rtype *>>(functypefixed4.args);
}
template <>
funcTypeFixed4 *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct funcTypeFixed64 : public Type {
    funcType funcType;
    std::vector<rtype *> args;
};
template <>
inline bool moku::is_nil<funcTypeFixed64>(
        const funcTypeFixed64 &functypefixed64)
{
    return moku::is_nil<funcType>(functypefixed64.funcType) &&
           moku::is_nil<std::vector<rtype *>>(functypefixed64.args);
}
template <>
funcTypeFixed64 *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct funcTypeFixed8 : public Type {
    funcType funcType;
    std::vector<rtype *> args;
};
template <>
inline bool moku::is_nil<funcTypeFixed8>(const funcTypeFixed8 &functypefixed8)
{
    return moku::is_nil<funcType>(functypefixed8.funcType) &&
           moku::is_nil<std::vector<rtype *>>(functypefixed8.args);
}
template <>
funcTypeFixed8 *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
std::tuple<Value, int, int> grow(Value s, int extra);
bool hasPrefix(std::string s, std::string prefix);
bool haveIdenticalType(Type T, Type V, bool cmpTags);
bool haveIdenticalUnderlyingType(rtype *T, rtype *V, bool cmpTags);
void ifaceE2I(rtype *t, moku::interface src, void *dst);
bool ifaceIndir(rtype *t);

struct imethod : public Type {
    nameOff name;
    typeOff typ;
};
template <>
inline bool moku::is_nil<imethod>(const imethod &imethod)
{
    return moku::is_nil<nameOff>(imethod.name) &&
           moku::is_nil<typeOff>(imethod.typ);
}
template <>
imethod *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
bool implements(rtype *T, rtype *V);

struct interfaceType : public Type {
    rtype rtype;
    name pkgPath;
    moku::slice<imethod> methods;
    Method Method(int i);
    int NumMethod();
    std::tuple<Method, bool> MethodByName(std::string name);
};
template <>
inline bool moku::is_nil<interfaceType>(const interfaceType &interfacetype)
{
    return moku::is_nil<rtype>(interfacetype.rtype) &&
           moku::is_nil<name>(interfacetype.pkgPath) &&
           moku::is_nil<moku::slice<imethod>>(interfacetype.methods);
}
template <>
interfaceType *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
bool isLetter(int32_t ch);
bool isReflexive(rtype *t);
bool isValidFieldName(std::string fieldName);
bool ismapkey(rtype *);
static constexpr int kindDirectIface{32};
static constexpr int kindGCProg{64};
static constexpr int kindMask{31};
static moku::slice<std::string> kindNames;
static constexpr int kindNoPointers{128};
static Map layoutCache;

struct layoutKey : public Type {
    rtype *t{std::nullptr};
    rtype *rcvr{std::nullptr};
};
template <>
inline bool moku::is_nil<layoutKey>(const layoutKey &layoutkey)
{
    return moku::is_nil<rtype *>(layoutkey.t) &&
           moku::is_nil<rtype *>(layoutkey.rcvr);
}
template <>
layoutKey *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct layoutType : public Type {
    rtype *t{std::nullptr};
    uintptr_t argSize{0};
    uintptr_t retOffset{0};
    bitVector *stack{std::nullptr};
    Pool *framePool{std::nullptr};
};
template <>
inline bool moku::is_nil<layoutType>(const layoutType &layouttype)
{
    return moku::is_nil<rtype *>(layouttype.t) &&
           moku::is_nil<uintptr_t>(layouttype.argSize) &&
           moku::is_nil<uintptr_t>(layouttype.retOffset) &&
           moku::is_nil<bitVector *>(layouttype.stack) &&
           moku::is_nil<Pool *>(layouttype.framePool);
}
template <>
layoutType *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
static Map lookupCache;
Value makeBytes(flag f, moku::slice<uint8_t> v, Type t);
Value makeComplex(flag f, moku::complex<double> v, Type t);
Value makeFloat(flag f, double v, Type t);

struct makeFuncImpl : public Type {
    uintptr_t code{0};
    bitVector *stack{std::nullptr};
    funcType *typ{std::nullptr};
    std::function<moku::slice<Value>(moku::slice<Value>)> fn{std::nullptr};
};
template <>
inline bool moku::is_nil<makeFuncImpl>(const makeFuncImpl &makefuncimpl)
{
    return moku::is_nil<uintptr_t>(makefuncimpl.code) &&
           moku::is_nil<bitVector *>(makefuncimpl.stack) &&
           moku::is_nil<funcType *>(makefuncimpl.typ) &&
           moku::is_nil<std::function<moku::slice<Value>(moku::slice<Value>)>>(
                   makefuncimpl.fn);
}
template <>
makeFuncImpl *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
void makeFuncStub();
Value makeInt(flag f, uint64_t bits, Type t);
Value makeMethodValue(std::string op, Value v);
Value makeRunes(flag f, moku::slice<int32_t> v, Type t);
Value makeString(flag f, std::string v, Type t);
void *makechan(rtype *typ, uint64_t size);
void *makemap(rtype *t, int cap);

struct mapType : public Type {
    rtype rtype;
    rtype *key{std::nullptr};
    rtype *elem{std::nullptr};
    rtype *bucket{std::nullptr};
    rtype *hmap{std::nullptr};
    uint8_t keysize{0};
    uint8_t indirectkey{0};
    uint8_t valuesize{0};
    uint8_t indirectvalue{0};
    uint16_t bucketsize{0};
    bool reflexivekey{false};
    bool needkeyupdate{false};
};
template <>
inline bool moku::is_nil<mapType>(const mapType &maptype)
{
    return moku::is_nil<rtype>(maptype.rtype) &&
           moku::is_nil<rtype *>(maptype.key) &&
           moku::is_nil<rtype *>(maptype.elem) &&
           moku::is_nil<rtype *>(maptype.bucket) &&
           moku::is_nil<rtype *>(maptype.hmap) &&
           moku::is_nil<uint8_t>(maptype.keysize) &&
           moku::is_nil<uint8_t>(maptype.indirectkey) &&
           moku::is_nil<uint8_t>(maptype.valuesize) &&
           moku::is_nil<uint8_t>(maptype.indirectvalue) &&
           moku::is_nil<uint16_t>(maptype.bucketsize) &&
           moku::is_nil<bool>(maptype.reflexivekey) &&
           moku::is_nil<bool>(maptype.needkeyupdate);
}
template <>
mapType *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
void *mapaccess(rtype *t, void *m, void *key);
void mapassign(rtype *t, void *m, void *key, void *val);
void mapdelete(rtype *t, void *m, void *key);
void *mapiterinit(rtype *t, void *m);
void *mapiterkey(void *it);
void mapiternext(void *it);
int maplen(void *m);
static constexpr uintptr_t maxKeySize{128};
static constexpr int maxPtrmaskBytes{2048};
static constexpr uintptr_t maxValSize{128};
void memclrNoHeapPointers(void *ptr, uintptr_t n);

struct method : public Type {
    nameOff name;
    typeOff mtyp;
    textOff ifn;
    textOff tfn;
};
template <>
inline bool moku::is_nil<method>(const method &method)
{
    return moku::is_nil<nameOff>(method.name) &&
           moku::is_nil<typeOff>(method.mtyp) &&
           moku::is_nil<textOff>(method.ifn) &&
           moku::is_nil<textOff>(method.tfn);
}
template <>
method *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
static Map methodCache;
std::string methodName();
std::tuple<rtype *, rtype *, void *> methodReceiver(std::string op,
                                                    Value v,
                                                    int methodIndex);

struct methodValue : public Type {
    uintptr_t fn{0};
    bitVector *stack{std::nullptr};
    int method{0};
    Value rcvr;
};
template <>
inline bool moku::is_nil<methodValue>(const methodValue &methodvalue)
{
    return moku::is_nil<uintptr_t>(methodvalue.fn) &&
           moku::is_nil<bitVector *>(methodvalue.stack) &&
           moku::is_nil<int>(methodvalue.method) &&
           moku::is_nil<Value>(methodvalue.rcvr);
}
template <>
methodValue *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
void methodValueCall();

struct name : public Type {
    uint8_t *bytes{std::nullptr};
    inline uint8_t *data(int off)
    {
        name name = *this;
        return name._dataByValue(int off);
    }
    uint8_t *_dataByValue(int off);
    inline bool isExported()
    {
        name name = *this;
        return name._isExportedByValue();
    }
    bool _isExportedByValue();
    inline int nameLen()
    {
        name name = *this;
        return name._nameLenByValue();
    }
    int _nameLenByValue();
    inline int tagLen()
    {
        name name = *this;
        return name._tagLenByValue();
    }
    int _tagLenByValue();
    inline std::string name()
    {
        name name = *this;
        return name._nameByValue();
    }
    std::string _nameByValue();
    inline std::string tag()
    {
        name name = *this;
        return name._tagByValue();
    }
    std::string _tagByValue();
    inline std::string pkgPath()
    {
        name name = *this;
        return name._pkgPathByValue();
    }
    std::string _pkgPathByValue();
};
template <>
inline bool moku::is_nil<name>(const name &name)
{
    return moku::is_nil<uint8_t *>(name.bytes);
}
template <>
name *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct nameOff : public moku::basic<int32_t>, public Type {
    nameOff() : moku::basic<int32_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<nameOff>(const nameOff &nameoff)
{
    return moku::is_nil<int32_t>(int32_t(nameoff));
}
template <>
nameOff *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
bool needKeyUpdate(rtype *t);
name newName(std::string n,
             std::string tag,
             std::string pkgPath,
             bool exported);

struct nonEmptyInterface : public Type {
    struct {
        rtype *ityp{std::nullptr};
        rtype *typ{std::nullptr};
        void *link{std::nullptr};
        int32_t bad{0};
        int32_t unused{0};
        std::vector<void *> fun;
    } * itab{std::nullptr};
    void *word{std::nullptr};
};
template <>
inline bool moku::is_nil<nonEmptyInterface>(
        const nonEmptyInterface &nonemptyinterface)
{
    return moku::is_nil < struct {
        rtype *ityp{std::nullptr};
        rtype *typ{std::nullptr};
        void *link{std::nullptr};
        int32_t bad{0};
        int32_t unused{0};
        std::vector<void *> fun;
    } * > (nonemptyinterface.itab) &&
            moku::is_nil<void *>(nonemptyinterface.word);
}
template <>
nonEmptyInterface *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
bool overflowFloat32(double x);
moku::interface packEface(Value v);
static Map ptrMap;
static constexpr int ptrSize{8};

struct ptrType : public Type {
    rtype rtype;
    rtype *elem{std::nullptr};
};
template <>
inline bool moku::is_nil<ptrType>(const ptrType &ptrtype)
{
    return moku::is_nil<rtype>(ptrtype.rtype) &&
           moku::is_nil<rtype *>(ptrtype.elem);
}
template <>
ptrType *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
void *resolveNameOff(void *ptrInModule, int32_t off);
nameOff resolveReflectName(name n);
textOff resolveReflectText(void *ptr);
typeOff resolveReflectType(rtype *t);
void *resolveTextOff(void *rtype, int32_t off);
void *resolveTypeOff(void *rtype, int32_t off);
uintptr_t round(uintptr_t n, uintptr_t a);
std::tuple<int, bool> rselect(moku::slice<runtimeSelect>);

struct rtype : public Type {
    uintptr_t size{0};
    uintptr_t ptrdata{0};
    uint32_t hash{0};
    tflag tflag;
    uint8_t align{0};
    uint8_t fieldAlign{0};
    uint8_t kind{0};
    typeAlg *alg{std::nullptr};
    uint8_t *gcdata{std::nullptr};
    nameOff str;
    typeOff ptrToThis;
    name nameOff(nameOff off);
    rtype *typeOff(typeOff off);
    void *textOff(textOff off);
    uncommonType *uncommon();
    std::string String();
    uintptr_t Size();
    int Bits();
    int Align();
    int FieldAlign();
    Kind Kind();
    bool pointers();
    rtype *common();
    moku::slice<method> exportedMethods();
    int NumMethod();
    Method Method(int i);
    std::tuple<Method, bool> MethodByName(std::string name);
    std::string PkgPath();
    std::string Name();
    ChanDir ChanDir();
    bool IsVariadic();
    Type Elem();
    StructField Field(int i);
    StructField FieldByIndex(moku::slice<int> index);
    std::tuple<StructField, bool> FieldByName(std::string name);
    std::tuple<StructField, bool> FieldByNameFunc(
            std::function<bool(std::string)> match);
    Type In(int i);
    Type Key();
    int Len();
    int NumField();
    int NumIn();
    int NumOut();
    Type Out(int i);
    rtype *ptrTo();
    bool Implements(Type u);
    bool AssignableTo(Type u);
    bool ConvertibleTo(Type u);
    bool Comparable();
};
template <>
inline bool moku::is_nil<rtype>(const rtype &rtype)
{
    return moku::is_nil<uintptr_t>(rtype.size) &&
           moku::is_nil<uintptr_t>(rtype.ptrdata) &&
           moku::is_nil<uint32_t>(rtype.hash) &&
           moku::is_nil<tflag>(rtype.tflag) &&
           moku::is_nil<uint8_t>(rtype.align) &&
           moku::is_nil<uint8_t>(rtype.fieldAlign) &&
           moku::is_nil<uint8_t>(rtype.kind) &&
           moku::is_nil<typeAlg *>(rtype.alg) &&
           moku::is_nil<uint8_t *>(rtype.gcdata) &&
           moku::is_nil<nameOff>(rtype.str) &&
           moku::is_nil<typeOff>(rtype.ptrToThis);
}
template <>
rtype *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
rtype *rtypeOff(void *section, int32_t off);

struct runtimeSelect : public Type {
    SelectDir dir;
    rtype *typ{std::nullptr};
    void *ch{std::nullptr};
    void *val{std::nullptr};
};
template <>
inline bool moku::is_nil<runtimeSelect>(const runtimeSelect &runtimeselect)
{
    return moku::is_nil<SelectDir>(runtimeselect.dir) &&
           moku::is_nil<rtype *>(runtimeselect.typ) &&
           moku::is_nil<void *>(runtimeselect.ch) &&
           moku::is_nil<void *>(runtimeselect.val);
}
template <>
runtimeSelect *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
structField runtimeStructField(StructField field);

struct sliceHeader : public Type {
    void *Data{std::nullptr};
    int Len{0};
    int Cap{0};
};
template <>
inline bool moku::is_nil<sliceHeader>(const sliceHeader &sliceheader)
{
    return moku::is_nil<void *>(sliceheader.Data) &&
           moku::is_nil<int>(sliceheader.Len) &&
           moku::is_nil<int>(sliceheader.Cap);
}
template <>
sliceHeader *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct sliceType : public Type {
    rtype rtype;
    rtype *elem{std::nullptr};
};
template <>
inline bool moku::is_nil<sliceType>(const sliceType &slicetype)
{
    return moku::is_nil<rtype>(slicetype.rtype) &&
           moku::is_nil<rtype *>(slicetype.elem);
}
template <>
sliceType *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
void storeRcvr(Value v, void *p);

struct stringHeader : public Type {
    void *Data{std::nullptr};
    int Len{0};
};
template <>
inline bool moku::is_nil<stringHeader>(const stringHeader &stringheader)
{
    return moku::is_nil<void *>(stringheader.Data) &&
           moku::is_nil<int>(stringheader.Len);
}
template <>
stringHeader *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct structField : public Type {
    name name;
    rtype *typ{std::nullptr};
    uintptr_t offsetAnon{0};
    uintptr_t offset();
    bool anon();
};
template <>
inline bool moku::is_nil<structField>(const structField &structfield)
{
    return moku::is_nil<name>(structfield.name) &&
           moku::is_nil<rtype *>(structfield.typ) &&
           moku::is_nil<uintptr_t>(structfield.offsetAnon);
}
template <>
structField *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
static struct {
    Mutex Mutex;
    Map m;
} structLookupCache;

struct structType : public Type {
    rtype rtype;
    name pkgPath;
    moku::slice<structField> fields;
    StructField Field(int i);
    StructField FieldByIndex(moku::slice<int> index);
    std::tuple<StructField, bool> FieldByNameFunc(
            std::function<bool(std::string)> match);
    std::tuple<StructField, bool> FieldByName(std::string name);
};
template <>
inline bool moku::is_nil<structType>(const structType &structtype)
{
    return moku::is_nil<rtype>(structtype.rtype) &&
           moku::is_nil<name>(structtype.pkgPath) &&
           moku::is_nil<moku::slice<structField>>(structtype.fields);
}
template <>
structType *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct structTypeFixed16 : public Type {
    structType structType;
    uncommonType u;
    std::vector<method> m;
};
template <>
inline bool moku::is_nil<structTypeFixed16>(
        const structTypeFixed16 &structtypefixed16)
{
    return moku::is_nil<structType>(structtypefixed16.structType) &&
           moku::is_nil<uncommonType>(structtypefixed16.u) &&
           moku::is_nil<std::vector<method>>(structtypefixed16.m);
}
template <>
structTypeFixed16 *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct structTypeFixed32 : public Type {
    structType structType;
    uncommonType u;
    std::vector<method> m;
};
template <>
inline bool moku::is_nil<structTypeFixed32>(
        const structTypeFixed32 &structtypefixed32)
{
    return moku::is_nil<structType>(structtypefixed32.structType) &&
           moku::is_nil<uncommonType>(structtypefixed32.u) &&
           moku::is_nil<std::vector<method>>(structtypefixed32.m);
}
template <>
structTypeFixed32 *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct structTypeFixed4 : public Type {
    structType structType;
    uncommonType u;
    std::vector<method> m;
};
template <>
inline bool moku::is_nil<structTypeFixed4>(
        const structTypeFixed4 &structtypefixed4)
{
    return moku::is_nil<structType>(structtypefixed4.structType) &&
           moku::is_nil<uncommonType>(structtypefixed4.u) &&
           moku::is_nil<std::vector<method>>(structtypefixed4.m);
}
template <>
structTypeFixed4 *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct structTypeFixed8 : public Type {
    structType structType;
    uncommonType u;
    std::vector<method> m;
};
template <>
inline bool moku::is_nil<structTypeFixed8>(
        const structTypeFixed8 &structtypefixed8)
{
    return moku::is_nil<structType>(structtypefixed8.structType) &&
           moku::is_nil<uncommonType>(structtypefixed8.u) &&
           moku::is_nil<std::vector<method>>(structtypefixed8.m);
}
template <>
structTypeFixed8 *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct structTypeUncommon : public Type {
    structType structType;
    uncommonType u;
};
template <>
inline bool moku::is_nil<structTypeUncommon>(
        const structTypeUncommon &structtypeuncommon)
{
    return moku::is_nil<structType>(structtypeuncommon.structType) &&
           moku::is_nil<uncommonType>(structtypeuncommon.u);
}
template <>
structTypeUncommon *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct textOff : public moku::basic<int32_t>, public Type {
    textOff() : moku::basic<int32_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<textOff>(const textOff &textoff)
{
    return moku::is_nil<int32_t>(int32_t(textoff));
}
template <>
textOff *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct tflag : public moku::basic<uint8_t>, public Type {
    tflag() : moku::basic<uint8_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<tflag>(const tflag &tflag)
{
    return moku::is_nil<uint8_t>(uint8_t(tflag));
}
template <>
tflag *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
static constexpr tflag tflagExtraStar{2};
static constexpr tflag tflagNamed{4};
static constexpr tflag tflagUncommon{1};
Type toType(rtype *t);

struct typeAlg : public Type {
    std::function<uintptr_t(void *, uintptr_t)> hash{std::nullptr};
    std::function<bool(void *, void *)> equal{std::nullptr};
};
template <>
inline bool moku::is_nil<typeAlg>(const typeAlg &typealg)
{
    return moku::is_nil<std::function<uintptr_t(void *, uintptr_t)>>(
                   typealg.hash) &&
           moku::is_nil<std::function<bool(void *, void *)>>(typealg.equal);
}
template <>
typeAlg *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}

struct typeOff : public moku::basic<int32_t>, public Type {
    typeOff() : moku::basic<int32_t>{0}
    {
    }
};
template <>
inline bool moku::is_nil<typeOff>(const typeOff &typeoff)
{
    return moku::is_nil<int32_t>(int32_t(typeoff));
}
template <>
typeOff *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
void typedmemmove(rtype *t, void *dst, void *src);
void typedmemmovepartial(
        rtype *t, void *dst, void *src, uintptr_t off, uintptr_t size);
int typedslicecopy(rtype *elemType, sliceHeader dst, sliceHeader src);
std::tuple<moku::slice<void *>, moku::slice<moku::slice<int32_t>>> typelinks();
uintptr_t typeptrdata(rtype *t);
moku::slice<rtype *> typesByString(std::string s);
void typesMustMatch(std::string what, Type t1, Type t2);
static rtype *uint8Type;

struct uncommonType : public Type {
    nameOff pkgPath;
    uint16_t mcount{0};
    uint16_t _{0};
    uint32_t moff{0};
    uint32_t _{0};
    moku::slice<method> methods();
};
template <>
inline bool moku::is_nil<uncommonType>(const uncommonType &uncommontype)
{
    return moku::is_nil<nameOff>(uncommontype.pkgPath) &&
           moku::is_nil<uint16_t>(uncommontype.mcount) &&
           moku::is_nil<uint16_t>(uncommontype._) &&
           moku::is_nil<uint32_t>(uncommontype.moff) &&
           moku::is_nil<uint32_t>(uncommontype._);
}
template <>
uncommonType *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
Value unpackEface(moku::interface i);
void *unsafe_New(rtype *);
void *unsafe_NewArray(rtype *, int);
moku::interface valueInterface(Value v, bool safe);

struct visit : public Type {
    void *a1{std::nullptr};
    void *a2{std::nullptr};
    Type typ;
};
template <>
inline bool moku::is_nil<visit>(const visit &visit)
{
    return moku::is_nil<void *>(visit.a1) && moku::is_nil<void *>(visit.a2) &&
           moku::is_nil<Type>(visit.typ);
}
template <>
visit *moku::try_assert(const moku::interface &iface)
{
    if (Type *type = moku::type_registry::try_assert<Type>(iface))
        return type;
    return std::nullptr;
}
} // namespace reflect
