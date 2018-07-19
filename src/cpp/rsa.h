#pragma once
#include "big.h"
#include "bytes.h"
#include "crypto.h"
#include "errors.h"
#include "hash.h"
#include "io.h"
#include "math.h"
#include "rand.h"
#include "subtle.h"
namespace rsa {

struct CRTValue {
    Int *Exp{std::nullptr};
    Int *Coeff{std::nullptr};
    Int *R{std::nullptr};
};
template <>
inline bool moku::is_nil<CRTValue>(const CRTValue &crtvalue)
{
    return moku::is_nil<Int *>(crtvalue.Exp) &&
           moku::is_nil<Int *>(crtvalue.Coeff) &&
           moku::is_nil<Int *>(crtvalue.R);
}
std::tuple<moku::slice<uint8_t>, moku::error> DecryptOAEP(
        Hash hash,
        Reader random,
        PrivateKey *priv,
        moku::slice<uint8_t> ciphertext,
        moku::slice<uint8_t> label);
std::tuple<moku::slice<uint8_t>, moku::error> DecryptPKCS1v15(
        Reader rand, PrivateKey *priv, moku::slice<uint8_t> ciphertext);
moku::error DecryptPKCS1v15SessionKey(Reader rand,
                                      PrivateKey *priv,
                                      moku::slice<uint8_t> ciphertext,
                                      moku::slice<uint8_t> key);
std::tuple<moku::slice<uint8_t>, moku::error> EncryptOAEP(
        Hash hash,
        Reader random,
        PublicKey *pub,
        moku::slice<uint8_t> msg,
        moku::slice<uint8_t> label);
std::tuple<moku::slice<uint8_t>, moku::error> EncryptPKCS1v15(
        Reader rand, PublicKey *pub, moku::slice<uint8_t> msg);
moku::error ErrDecryption;
moku::error ErrMessageTooLong;
moku::error ErrVerification;
std::tuple<PrivateKey *, moku::error> GenerateKey(Reader random, int bits);
std::tuple<PrivateKey *, moku::error> GenerateMultiPrimeKey(Reader random,
                                                            int nprimes,
                                                            int bits);

struct OAEPOptions {
    Hash Hash;
    moku::slice<uint8_t> Label;
};
template <>
inline bool moku::is_nil<OAEPOptions>(const OAEPOptions &oaepoptions)
{
    return moku::is_nil<Hash>(oaepoptions.Hash) &&
           moku::is_nil<moku::slice<uint8_t>>(oaepoptions.Label);
}
struct PKCS1v15DecryptOptions {
    int SessionKeyLen{0};
};
template <>
inline bool moku::is_nil<PKCS1v15DecryptOptions>(
        const PKCS1v15DecryptOptions &pkcs1v15decryptoptions)
{
    return moku::is_nil<int>(pkcs1v15decryptoptions.SessionKeyLen);
}
struct PSSOptions {
    int SaltLength{0};
    Hash Hash;
    Hash HashFunc();
    int saltLength();
};
template <>
inline bool moku::is_nil<PSSOptions>(const PSSOptions &pssoptions)
{
    return moku::is_nil<int>(pssoptions.SaltLength) &&
           moku::is_nil<Hash>(pssoptions.Hash);
}
constexpr int PSSSaltLengthAuto{0};
constexpr int PSSSaltLengthEqualsHash{-1};

struct PrecomputedValues {
    Int *Dp{std::nullptr};
    Int *Dq{std::nullptr};
    Int *Qinv{std::nullptr};
    moku::slice<CRTValue> CRTValues;
};
template <>
inline bool moku::is_nil<PrecomputedValues>(
        const PrecomputedValues &precomputedvalues)
{
    return moku::is_nil<Int *>(precomputedvalues.Dp) &&
           moku::is_nil<Int *>(precomputedvalues.Dq) &&
           moku::is_nil<Int *>(precomputedvalues.Qinv) &&
           moku::is_nil<moku::slice<CRTValue>>(precomputedvalues.CRTValues);
}
struct PrivateKey {
    PublicKey PublicKey;
    Int *D{std::nullptr};
    moku::slice<Int *> Primes;
    PrecomputedValues Precomputed;
    PublicKey Public();
    std::tuple<moku::slice<uint8_t>, moku::error> Sign(Reader rand,
                                                       moku::slice<uint8_t> msg,
                                                       SignerOpts opts);
    std::tuple<moku::slice<uint8_t>, moku::error> Decrypt(
            Reader rand, moku::slice<uint8_t> ciphertext, DecrypterOpts opts);
    moku::error Validate();
    void Precompute();
};
template <>
inline bool moku::is_nil<PrivateKey>(const PrivateKey &privatekey)
{
    return moku::is_nil<PublicKey>(privatekey.PublicKey) &&
           moku::is_nil<Int *>(privatekey.D) &&
           moku::is_nil<moku::slice<Int *>>(privatekey.Primes) &&
           moku::is_nil<PrecomputedValues>(privatekey.Precomputed);
}
struct PublicKey {
    Int *N{std::nullptr};
    int E{0};
};
template <>
inline bool moku::is_nil<PublicKey>(const PublicKey &publickey)
{
    return moku::is_nil<Int *>(publickey.N) && moku::is_nil<int>(publickey.E);
}
std::tuple<moku::slice<uint8_t>, moku::error> SignPKCS1v15(
        Reader rand, PrivateKey *priv, Hash hash, moku::slice<uint8_t> hashed);
std::tuple<moku::slice<uint8_t>, moku::error> SignPSS(
        Reader rand,
        PrivateKey *priv,
        Hash hash,
        moku::slice<uint8_t> hashed,
        PSSOptions *opts);
moku::error VerifyPKCS1v15(PublicKey *pub,
                           Hash hash,
                           moku::slice<uint8_t> hashed,
                           moku::slice<uint8_t> sig);
moku::error VerifyPSS(PublicKey *pub,
                      Hash hash,
                      moku::slice<uint8_t> hashed,
                      moku::slice<uint8_t> sig,
                      PSSOptions *opts);
static Int *bigOne;
static Int *bigZero;
moku::error checkPub(PublicKey *pub);
void copyWithLeftPad(moku::slice<uint8_t> dest, moku::slice<uint8_t> src);
std::tuple<Int *, moku::error> decrypt(Reader random, PrivateKey *priv, Int *c);
std::tuple<Int *, moku::error> decryptAndCheck(Reader random,
                                               PrivateKey *priv,
                                               Int *c);
std::tuple<int, moku::slice<uint8_t>, int, moku::error> decryptPKCS1v15(
        Reader rand, PrivateKey *priv, moku::slice<uint8_t> ciphertext);
std::tuple<moku::slice<uint8_t>, moku::error> emsaPSSEncode(
        moku::slice<uint8_t> mHash,
        int emBits,
        moku::slice<uint8_t> salt,
        Hash hash);
moku::error emsaPSSVerify(moku::slice<uint8_t> mHash,
                          moku::slice<uint8_t> em,
                          int emBits,
                          int sLen,
                          Hash hash);
Int *encrypt(Int *c, PublicKey *pub, Int *m);
static moku::error errPublicExponentLarge;
static moku::error errPublicExponentSmall;
static moku::error errPublicModulus;
static std::map<Hash, moku::slice<uint8_t>> hashPrefixes;
void incCounter(std::vector<uint8_t> *c);
moku::slice<uint8_t> leftPad(moku::slice<uint8_t> input, int size);
void mgf1XOR(moku::slice<uint8_t> out, Hash hash, moku::slice<uint8_t> seed);
std::tuple<Int *, bool> modInverse(Int *a, Int *n);
moku::error nonZeroRandomBytes(moku::slice<uint8_t> s, Reader rand);
std::tuple<int, moku::slice<uint8_t>, moku::error> pkcs1v15HashInfo(Hash hash,
                                                                    int inLen);
std::tuple<moku::slice<uint8_t>, moku::error> signPSSWithSalt(
        Reader rand,
        PrivateKey *priv,
        Hash hash,
        moku::slice<uint8_t> hashed,
        moku::slice<uint8_t> salt);
moku::error verifyPSS(PublicKey *pub,
                      Hash hash,
                      moku::slice<uint8_t> hashed,
                      moku::slice<uint8_t> sig,
                      int saltLen);
} // namespace rsa
