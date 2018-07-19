#pragma once
#include "aes.h"
#include "asn1.h"
#include "big.h"
#include "cipher.h"
#include "crypto.h"
#include "elliptic.h"
#include "errors.h"
#include "io.h"
#include "sha512.h"
namespace ecdsa {
std::tuple<PrivateKey *, moku::error> GenerateKey(Curve c, Reader rand);

struct PrivateKey {
    PublicKey PublicKey;
    Int *D{std::nullptr};
    PublicKey Public();
    std::tuple<moku::slice<uint8_t>, moku::error> Sign(Reader rand,
                                                       moku::slice<uint8_t> msg,
                                                       SignerOpts opts);
};
template <>
inline bool moku::is_nil<PrivateKey>(const PrivateKey &privatekey)
{
    return moku::is_nil<PublicKey>(privatekey.PublicKey) &&
           moku::is_nil<Int *>(privatekey.D);
}
struct PublicKey {
    Curve Curve;
    Int *X{std::nullptr};
    Int *Y{std::nullptr};
};
template <>
inline bool moku::is_nil<PublicKey>(const PublicKey &publickey)
{
    return moku::is_nil<Curve>(publickey.Curve) &&
           moku::is_nil<Int *>(publickey.X) && moku::is_nil<Int *>(publickey.Y);
}
std::tuple<Int *, Int *, moku::error> Sign(Reader rand,
                                           PrivateKey *priv,
                                           moku::slice<uint8_t> hash);
bool Verify(PublicKey *pub, moku::slice<uint8_t> hash, Int *r, Int *s);
static constexpr std::string aesIV{"IV for ECDSA CTR"};

struct combinedMult {
    virtual std::tuple<Int *, Int *> CombinedMult(
            Int *bigX,
            Int *bigY,
            moku::slice<uint8_t> baseScalar,
            moku::slice<uint8_t> scalar) = 0;
};

struct ecdsaSignature {
    Int *R{std::nullptr};
    Int *S{std::nullptr};
};
template <>
inline bool moku::is_nil<ecdsaSignature>(const ecdsaSignature &ecdsasignature)
{
    return moku::is_nil<Int *>(ecdsasignature.R) &&
           moku::is_nil<Int *>(ecdsasignature.S);
}
static moku::error errZeroParam;
Int *fermatInverse(Int *k, Int *N);
Int *hashToInt(moku::slice<uint8_t> hash, Curve c);

struct invertible {
    virtual Int *Inverse(Int *k) = 0;
};
static Int *one;
std::tuple<Int *, moku::error> randFieldElement(Curve c, Reader rand);
static zr *zeroReader;

struct zr {
    Reader Reader;
    std::tuple<int, moku::error> Read(moku::slice<uint8_t> dst);
};
template <>
inline bool moku::is_nil<zr>(const zr &zr)
{
    return moku::is_nil<Reader>(zr.Reader);
}
} // namespace ecdsa
