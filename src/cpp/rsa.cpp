std::tuple<moku::slice<uint8_t>, moku::error> EncryptPKCS1v15(
        Reader rand, PublicKey *pub, moku::slice<uint8_t> msg)
{
    Int *c{std::nullptr};
    moku::slice<uint8_t> em{};
    moku::error err{};
    int k{0};
    Int *m{std::nullptr};
    moku::slice<uint8_t> mm{};
    moku::slice<uint8_t> ps{};

    {
        moku::error err{};

        err = checkPub(pub);

        if (!moku::is_nil(err)) {
            return {nil, err};
        }
    }
    k = (pub->N->BitLen() + 7) / 8;
    if (len(msg) > k - 11) {
        return {nil, ErrMessageTooLong};
    }
    em = make(moku::slice<byte>, k);
    em[1] = 2;
    std::tie(ps, mm) = std::tuple<moku::slice<uint8_t>, moku::slice<uint8_t>>(
            moku::slice_expr<moku::slice<uint8_t>>(
                    em, 2, len(em) - len(msg) - 1),
            moku::slice_expr<moku::slice<uint8_t>>(em, len(em) - len(msg)));
    err = nonZeroRandomBytes(ps, rand);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    em[len(em) - len(msg) - 1] = 0;
    copy(mm, msg);
    m = new (big::Int)->SetBytes(em);
    c = encrypt(new (big::Int), pub, m);
    copyWithLeftPad(em, c->Bytes());
    return {em, nil};
}
std::tuple<moku::slice<uint8_t>, moku::error> DecryptPKCS1v15(
        Reader rand, PrivateKey *priv, moku::slice<uint8_t> ciphertext)
{
    moku::error err{};
    int index{0};
    moku::slice<uint8_t> out{};
    int valid{0};

    {
        moku::error err{};

        err = checkPub(&priv->PublicKey);

        if (!moku::is_nil(err)) {
            return {nil, err};
        }
    }
    std::tie(valid, out, index, err) = decryptPKCS1v15(rand, priv, ciphertext);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    if (valid == 0) {
        return {nil, ErrDecryption};
    }
    return {moku::slice_expr<moku::slice<uint8_t>>(out, index), nil};
}
moku::error DecryptPKCS1v15SessionKey(Reader rand,
                                      PrivateKey *priv,
                                      moku::slice<uint8_t> ciphertext,
                                      moku::slice<uint8_t> key)
{
    moku::slice<uint8_t> em{};
    moku::error err{};
    int index{0};
    int k{0};
    int valid{0};

    {
        moku::error err{};

        err = checkPub(&priv->PublicKey);

        if (!moku::is_nil(err)) {
            return err;
        }
    }
    k = (priv->N->BitLen() + 7) / 8;
    if (k - (len(key) + 3 + 8) < 0) {
        return ErrDecryption;
    }
    std::tie(valid, em, index, err) = decryptPKCS1v15(rand, priv, ciphertext);
    if (!moku::is_nil(err)) {
        return err;
    }
    if (len(em) != k) {
        return ErrDecryption;
    }
    valid &=
            subtle::ConstantTimeEq(int32_t(len(em) - index), int32_t(len(key)));
    subtle::ConstantTimeCopy(
            valid,
            key,
            moku::slice_expr<moku::slice<uint8_t>>(em, len(em) - len(key)));
    return nil;
}
std::tuple<int, moku::slice<uint8_t>, int, moku::error> decryptPKCS1v15(
        Reader rand, PrivateKey *priv, moku::slice<uint8_t> ciphertext)
{
    Int *c{std::nullptr};
    moku::slice<uint8_t> em{};
    moku::error err{};
    int firstByteIsZero{0};
    int index{0};
    int k{0};
    int lookingForIndex{0};
    Int *m{std::nullptr};
    int secondByteIsTwo{0};
    int valid{0};
    int validPS{0};

    k = (priv->N->BitLen() + 7) / 8;
    if (k < 11) {
        err = ErrDecryption;
        return;
    }
    c = new (big::Int)->SetBytes(ciphertext);
    std::tie(m, err) = decrypt(rand, priv, c);
    if (!moku::is_nil(err)) {
        return;
    }
    em = leftPad(m->Bytes(), k);
    firstByteIsZero = subtle::ConstantTimeByteEq(em[0], 0);
    secondByteIsTwo = subtle::ConstantTimeByteEq(em[1], 2);
    lookingForIndex = 1;
    {
        int i{0};
        for (i = 2; i < len(em); i++) {
            int equals0{0};

            equals0 = subtle::ConstantTimeByteEq(em[i], 0);
            index = subtle::ConstantTimeSelect(
                    lookingForIndex & equals0, i, index);
            lookingForIndex =
                    subtle::ConstantTimeSelect(equals0, 0, lookingForIndex);
        }
    }
    validPS = subtle::ConstantTimeLessOrEq(2 + 8, index);
    valid = firstByteIsZero & secondByteIsTwo & (^lookingForIndex & 1) &
            validPS;
    index = subtle::ConstantTimeSelect(valid, index + 1, 0);
    return {valid, em, index, nil};
}
moku::error nonZeroRandomBytes(moku::slice<uint8_t> s, Reader rand)
{
    moku::error err{};

    std::tie(_, err) = io::ReadFull(rand, s);
    if (!moku::is_nil(err)) {
        return;
    }
    {
        int i{0};
        for (i = 0; i < len(s); i++) {
            while (s[i] == 0) {
                std::tie(_, err) = io::ReadFull(
                        rand,
                        moku::slice_expr<moku::slice<uint8_t>>(s, i, i + 1));
                if (!moku::is_nil(err)) {
                    return;
                }
                s[i] ^= 0x42;
            }
        }
    }
    return;
}
std::tuple<moku::slice<uint8_t>, moku::error> SignPKCS1v15(
        Reader rand, PrivateKey *priv, Hash hash, moku::slice<uint8_t> hashed)
{
    Int *c{std::nullptr};
    moku::slice<uint8_t> em{};
    moku::error err{};
    int hashLen{0};
    int k{0};
    Int *m{std::nullptr};
    moku::slice<uint8_t> prefix{};
    int tLen{0};

    std::tie(hashLen, prefix, err) = pkcs1v15HashInfo(hash, len(hashed));
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    tLen = len(prefix) + hashLen;
    k = (priv->N->BitLen() + 7) / 8;
    if (k < tLen + 11) {
        return {nil, ErrMessageTooLong};
    }
    em = make(moku::slice<byte>, k);
    em[1] = 1;
    {
        int i{0};
        for (i = 2; i < k - tLen - 1; i++) {
            em[i] = 0xff;
        }
    }
    copy(moku::slice_expr<moku::slice<uint8_t>>(em, k - tLen, k - hashLen),
         prefix);
    copy(moku::slice_expr<moku::slice<uint8_t>>(em, k - hashLen, k), hashed);
    m = new (big::Int)->SetBytes(em);
    std::tie(c, err) = decryptAndCheck(rand, priv, m);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    copyWithLeftPad(em, c->Bytes());
    return {em, nil};
}
moku::error VerifyPKCS1v15(PublicKey *pub,
                           Hash hash,
                           moku::slice<uint8_t> hashed,
                           moku::slice<uint8_t> sig)
{
    Int *c{std::nullptr};
    moku::slice<uint8_t> em{};
    moku::error err{};
    int hashLen{0};
    int k{0};
    Int *m{std::nullptr};
    int ok{0};
    moku::slice<uint8_t> prefix{};
    int tLen{0};

    std::tie(hashLen, prefix, err) = pkcs1v15HashInfo(hash, len(hashed));
    if (!moku::is_nil(err)) {
        return err;
    }
    tLen = len(prefix) + hashLen;
    k = (pub->N->BitLen() + 7) / 8;
    if (k < tLen + 11) {
        return ErrVerification;
    }
    c = new (big::Int)->SetBytes(sig);
    m = encrypt(new (big::Int), pub, c);
    em = leftPad(m->Bytes(), k);
    ok = subtle::ConstantTimeByteEq(em[0], 0);
    ok &= subtle::ConstantTimeByteEq(em[1], 1);
    ok &= subtle::ConstantTimeCompare(
            moku::slice_expr<moku::slice<uint8_t>>(em, k - hashLen, k), hashed);
    ok &= subtle::ConstantTimeCompare(
            moku::slice_expr<moku::slice<uint8_t>>(em, k - tLen, k - hashLen),
            prefix);
    ok &= subtle::ConstantTimeByteEq(em[k - tLen - 1], 0);
    {
        int i{0};
        for (i = 2; i < k - tLen - 1; i++) {
            ok &= subtle::ConstantTimeByteEq(em[i], 0xff);
        }
    }
    if (ok != 1) {
        return ErrVerification;
    }
    return nil;
}
std::tuple<int, moku::slice<uint8_t>, moku::error> pkcs1v15HashInfo(Hash hash,
                                                                    int inLen)
{
    moku::error err{};
    int hashLen{0};
    bool ok{false};
    moku::slice<uint8_t> prefix{};

    if (hash == 0) {
        return {inLen, nil, nil};
    }
    hashLen = hash.Size();
    if (inLen != hashLen) {
        return {0,
                nil,
                errors::New("crypto/rsa: input must be hashed message")};
    }
    std::tie(prefix, ok) = hashPrefixes[hash];
    if (!ok) {
        return {0, nil, errors::New("crypto/rsa: unsupported hash function")};
    }
    return;
}
void copyWithLeftPad(moku::slice<uint8_t> dest, moku::slice<uint8_t> src)
{
    int numPaddingBytes{0};

    numPaddingBytes = len(dest) - len(src);
    {
        int i{0};
        for (i = 0; i < numPaddingBytes; i++) {
            dest[i] = 0;
        }
    }
    copy(moku::slice_expr<moku::slice<uint8_t>>(dest, numPaddingBytes), src);
}
int main()
{
    hashPrefixes =
            std::map<crypto::Hash, moku::slice<byte>>{{crypto::MD5,
                                                       {0x30,
                                                        0x20,
                                                        0x30,
                                                        0x0c,
                                                        0x06,
                                                        0x08,
                                                        0x2a,
                                                        0x86,
                                                        0x48,
                                                        0x86,
                                                        0xf7,
                                                        0x0d,
                                                        0x02,
                                                        0x05,
                                                        0x05,
                                                        0x00,
                                                        0x04,
                                                        0x10}},
                                                      {crypto::SHA1,
                                                       {0x30,
                                                        0x21,
                                                        0x30,
                                                        0x09,
                                                        0x06,
                                                        0x05,
                                                        0x2b,
                                                        0x0e,
                                                        0x03,
                                                        0x02,
                                                        0x1a,
                                                        0x05,
                                                        0x00,
                                                        0x04,
                                                        0x14}},
                                                      {crypto::SHA224,
                                                       {0x30,
                                                        0x2d,
                                                        0x30,
                                                        0x0d,
                                                        0x06,
                                                        0x09,
                                                        0x60,
                                                        0x86,
                                                        0x48,
                                                        0x01,
                                                        0x65,
                                                        0x03,
                                                        0x04,
                                                        0x02,
                                                        0x04,
                                                        0x05,
                                                        0x00,
                                                        0x04,
                                                        0x1c}},
                                                      {crypto::SHA256,
                                                       {0x30,
                                                        0x31,
                                                        0x30,
                                                        0x0d,
                                                        0x06,
                                                        0x09,
                                                        0x60,
                                                        0x86,
                                                        0x48,
                                                        0x01,
                                                        0x65,
                                                        0x03,
                                                        0x04,
                                                        0x02,
                                                        0x01,
                                                        0x05,
                                                        0x00,
                                                        0x04,
                                                        0x20}},
                                                      {crypto::SHA384,
                                                       {0x30,
                                                        0x41,
                                                        0x30,
                                                        0x0d,
                                                        0x06,
                                                        0x09,
                                                        0x60,
                                                        0x86,
                                                        0x48,
                                                        0x01,
                                                        0x65,
                                                        0x03,
                                                        0x04,
                                                        0x02,
                                                        0x02,
                                                        0x05,
                                                        0x00,
                                                        0x04,
                                                        0x30}},
                                                      {crypto::SHA512,
                                                       {0x30,
                                                        0x51,
                                                        0x30,
                                                        0x0d,
                                                        0x06,
                                                        0x09,
                                                        0x60,
                                                        0x86,
                                                        0x48,
                                                        0x01,
                                                        0x65,
                                                        0x03,
                                                        0x04,
                                                        0x02,
                                                        0x03,
                                                        0x05,
                                                        0x00,
                                                        0x04,
                                                        0x40}},
                                                      {crypto::MD5SHA1, {}},
                                                      {crypto::RIPEMD160,
                                                       {0x30,
                                                        0x20,
                                                        0x30,
                                                        0x08,
                                                        0x06,
                                                        0x06,
                                                        0x28,
                                                        0xcf,
                                                        0x06,
                                                        0x03,
                                                        0x00,
                                                        0x31,
                                                        0x04,
                                                        0x14}}};
    bigZero = big::NewInt(0);
    bigOne = big::NewInt(1);
    errPublicModulus = errors::New("crypto/rsa: missing public modulus");
    errPublicExponentSmall =
            errors::New("crypto/rsa: public exponent too small");
    errPublicExponentLarge =
            errors::New("crypto/rsa: public exponent too large");
    ErrMessageTooLong =
            errors::New("crypto/rsa: message too long for RSA public key size");
    ErrDecryption = errors::New("crypto/rsa: decryption error");
    ErrVerification = errors::New("crypto/rsa: verification error");
    _main();
    return 0;
}
std::tuple<moku::slice<uint8_t>, moku::error> emsaPSSEncode(
        moku::slice<uint8_t> mHash,
        int emBits,
        moku::slice<uint8_t> salt,
        Hash hash)
{
    moku::slice<uint8_t> db{};
    moku::slice<uint8_t> em{};
    int emLen{0};
    moku::slice<uint8_t> h{};
    int hLen{0};
    std::vector<uint8_t> prefix{};
    int sLen{0};

    hLen = hash::Size();
    sLen = len(salt);
    emLen = (emBits + 7) / 8;
    if (len(mHash) != hLen) {
        return {nil, errors::New("crypto/rsa: input must be hashed message")};
    }
    if (emLen < hLen + sLen + 2) {
        return {nil, errors::New("crypto/rsa: encoding error")};
    }
    em = make(moku::slice<byte>, emLen);
    db = moku::slice_expr<moku::slice<uint8_t>>(
            em, emLen - sLen - hLen - 2 + 1 + sLen);
    h = moku::slice_expr<moku::slice<uint8_t>>(
            em, emLen - sLen - hLen - 2 + 1 + sLen, emLen - 1);
    hash.Write(moku::slice_expr<std::vector<uint8_t>>(prefix));
    hash.Write(mHash);
    hash.Write(salt);
    h = hash::Sum(moku::slice_expr<moku::slice<uint8_t>>(h, 0));
    hash::Reset();
    db[emLen - sLen - hLen - 2] = 0x01;
    copy(moku::slice_expr<moku::slice<uint8_t>>(db, emLen - sLen - hLen - 1),
         salt);
    mgf1XOR(db, hash, h);
    db[0] &= (0xFF >> unsigned int(8 * emLen - emBits));
    em[emLen - 1] = 0xBC;
    return {em, nil};
}
moku::error emsaPSSVerify(moku::slice<uint8_t> mHash,
                          moku::slice<uint8_t> em,
                          int emBits,
                          int sLen,
                          Hash hash)
{
    moku::slice<uint8_t> db{};
    int emLen{0};
    moku::slice<uint8_t> h{};
    moku::slice<uint8_t> h0{};
    int hLen{0};
    std::vector<uint8_t> prefix{};
    moku::slice<uint8_t> salt{};

    hLen = hash::Size();
    if (hLen != len(mHash)) {
        return ErrVerification;
    }
    emLen = (emBits + 7) / 8;
    if (emLen < hLen + sLen + 2) {
        return ErrVerification;
    }
    if (em[len(em) - 1] != 0xBC) {
        return ErrVerification;
    }
    db = moku::slice_expr<moku::slice<uint8_t>>(em, emLen - hLen - 1);
    h = moku::slice_expr<moku::slice<uint8_t>>(
            em, emLen - hLen - 1, len(em) - 1);
    if (em[0] & (0xFF << unsigned int(8 - (8 * emLen - emBits))) != 0) {
        return ErrVerification;
    }
    mgf1XOR(db, hash, h);
    db[0] &= (0xFF >> unsigned int(8 * emLen - emBits));
    if (sLen == PSSSaltLengthAuto) {
    FindSaltLength:;
        if (sLen < 0) {
            return ErrVerification;
        }
    } else {
        {
            uint8_t e{0};
            for (e : moku::range_value<moku::slice<uint8_t>>(
                         moku::slice_expr<moku::slice<uint8_t>>(
                                 db, emLen - hLen - sLen - 2))) {
                if (e != 0x00) {
                    return ErrVerification;
                }
            }
        }
        if (db[emLen - hLen - sLen - 2] != 0x01) {
            return ErrVerification;
        }
    }
    salt = moku::slice_expr<moku::slice<uint8_t>>(db, len(db) - sLen);
    hash.Write(moku::slice_expr<std::vector<uint8_t>>(prefix));
    hash.Write(mHash);
    hash.Write(salt);
    h0 = hash::Sum(nil);
    if (!bytes::Equal(h0, h)) {
        return ErrVerification;
    }
    return nil;
}
std::tuple<moku::slice<uint8_t>, moku::error> signPSSWithSalt(
        Reader rand,
        PrivateKey *priv,
        Hash hash,
        moku::slice<uint8_t> hashed,
        moku::slice<uint8_t> salt)
{
    Int *c{std::nullptr};
    moku::slice<uint8_t> em{};
    moku::error err{};
    Int *m{std::nullptr};
    int nBits{0};
    moku::slice<uint8_t> s{};

    nBits = priv->N->BitLen();
    std::tie(em, err) = emsaPSSEncode(hashed, nBits - 1, salt, hash.New());
    if (!moku::is_nil(err)) {
        return;
    }
    m = new (big::Int)->SetBytes(em);
    std::tie(c, err) = decryptAndCheck(rand, priv, m);
    if (!moku::is_nil(err)) {
        return;
    }
    s = make(moku::slice<byte>, (nBits + 7) / 8);
    copyWithLeftPad(s, c->Bytes());
    return;
}
Hash PSSOptions::HashFunc()
{
    return this->Hash;
}
int PSSOptions::saltLength()
{
    if (moku::is_nil(this)) {
        return PSSSaltLengthAuto;
    }
    return this->SaltLength;
}
std::tuple<moku::slice<uint8_t>, moku::error> SignPSS(
        Reader rand,
        PrivateKey *priv,
        Hash hash,
        moku::slice<uint8_t> hashed,
        PSSOptions *opts)
{
    moku::slice<uint8_t> salt{};
    int saltLength{0};

    saltLength = opts->saltLength();
    if ((saltLength == PSSSaltLengthAuto)) {
    _ident_0_:
        saltLength = (priv->N->BitLen() + 7) / 8 - 2 - hash.Size();
    } else if ((saltLength == PSSSaltLengthEqualsHash)) {
    _ident_1_:
        saltLength = hash.Size();
    }
    if (!moku::is_nil(opts) && opts->Hash != 0) {
        hash = opts->Hash;
    }
    salt = make(moku::slice<byte>, saltLength);
    {
        moku::error err{};

        std::tie(_, err) = io::ReadFull(rand, salt);

        if (!moku::is_nil(err)) {
            return {nil, err};
        }
    }
    return signPSSWithSalt(rand, priv, hash, hashed, salt);
}
moku::error VerifyPSS(PublicKey *pub,
                      Hash hash,
                      moku::slice<uint8_t> hashed,
                      moku::slice<uint8_t> sig,
                      PSSOptions *opts)
{
    return verifyPSS(pub, hash, hashed, sig, opts->saltLength());
}
moku::error verifyPSS(PublicKey *pub,
                      Hash hash,
                      moku::slice<uint8_t> hashed,
                      moku::slice<uint8_t> sig,
                      int saltLen)
{
    moku::slice<uint8_t> em{};
    int emBits{0};
    int emLen{0};
    Int *m{std::nullptr};
    int nBits{0};
    Int *s{std::nullptr};

    nBits = pub->N->BitLen();
    if (len(sig) != (nBits + 7) / 8) {
        return ErrVerification;
    }
    s = new (big::Int)->SetBytes(sig);
    m = encrypt(new (big::Int), pub, s);
    emBits = nBits - 1;
    emLen = (emBits + 7) / 8;
    if (emLen < len(m->Bytes())) {
        return ErrVerification;
    }
    em = make(moku::slice<byte>, emLen);
    copyWithLeftPad(em, m->Bytes());
    if (saltLen == PSSSaltLengthEqualsHash) {
        saltLen = hash.Size();
    }
    return emsaPSSVerify(hashed, em, emBits, saltLen, hash.New());
}
int main()
{
    hashPrefixes =
            std::map<crypto::Hash, moku::slice<byte>>{{crypto::MD5,
                                                       {0x30,
                                                        0x20,
                                                        0x30,
                                                        0x0c,
                                                        0x06,
                                                        0x08,
                                                        0x2a,
                                                        0x86,
                                                        0x48,
                                                        0x86,
                                                        0xf7,
                                                        0x0d,
                                                        0x02,
                                                        0x05,
                                                        0x05,
                                                        0x00,
                                                        0x04,
                                                        0x10}},
                                                      {crypto::SHA1,
                                                       {0x30,
                                                        0x21,
                                                        0x30,
                                                        0x09,
                                                        0x06,
                                                        0x05,
                                                        0x2b,
                                                        0x0e,
                                                        0x03,
                                                        0x02,
                                                        0x1a,
                                                        0x05,
                                                        0x00,
                                                        0x04,
                                                        0x14}},
                                                      {crypto::SHA224,
                                                       {0x30,
                                                        0x2d,
                                                        0x30,
                                                        0x0d,
                                                        0x06,
                                                        0x09,
                                                        0x60,
                                                        0x86,
                                                        0x48,
                                                        0x01,
                                                        0x65,
                                                        0x03,
                                                        0x04,
                                                        0x02,
                                                        0x04,
                                                        0x05,
                                                        0x00,
                                                        0x04,
                                                        0x1c}},
                                                      {crypto::SHA256,
                                                       {0x30,
                                                        0x31,
                                                        0x30,
                                                        0x0d,
                                                        0x06,
                                                        0x09,
                                                        0x60,
                                                        0x86,
                                                        0x48,
                                                        0x01,
                                                        0x65,
                                                        0x03,
                                                        0x04,
                                                        0x02,
                                                        0x01,
                                                        0x05,
                                                        0x00,
                                                        0x04,
                                                        0x20}},
                                                      {crypto::SHA384,
                                                       {0x30,
                                                        0x41,
                                                        0x30,
                                                        0x0d,
                                                        0x06,
                                                        0x09,
                                                        0x60,
                                                        0x86,
                                                        0x48,
                                                        0x01,
                                                        0x65,
                                                        0x03,
                                                        0x04,
                                                        0x02,
                                                        0x02,
                                                        0x05,
                                                        0x00,
                                                        0x04,
                                                        0x30}},
                                                      {crypto::SHA512,
                                                       {0x30,
                                                        0x51,
                                                        0x30,
                                                        0x0d,
                                                        0x06,
                                                        0x09,
                                                        0x60,
                                                        0x86,
                                                        0x48,
                                                        0x01,
                                                        0x65,
                                                        0x03,
                                                        0x04,
                                                        0x02,
                                                        0x03,
                                                        0x05,
                                                        0x00,
                                                        0x04,
                                                        0x40}},
                                                      {crypto::MD5SHA1, {}},
                                                      {crypto::RIPEMD160,
                                                       {0x30,
                                                        0x20,
                                                        0x30,
                                                        0x08,
                                                        0x06,
                                                        0x06,
                                                        0x28,
                                                        0xcf,
                                                        0x06,
                                                        0x03,
                                                        0x00,
                                                        0x31,
                                                        0x04,
                                                        0x14}}};
    bigZero = big::NewInt(0);
    bigOne = big::NewInt(1);
    errPublicModulus = errors::New("crypto/rsa: missing public modulus");
    errPublicExponentSmall =
            errors::New("crypto/rsa: public exponent too small");
    errPublicExponentLarge =
            errors::New("crypto/rsa: public exponent too large");
    ErrMessageTooLong =
            errors::New("crypto/rsa: message too long for RSA public key size");
    ErrDecryption = errors::New("crypto/rsa: decryption error");
    ErrVerification = errors::New("crypto/rsa: verification error");
    _main();
    return 0;
}
moku::error checkPub(PublicKey *pub)
{
    if (moku::is_nil(pub->N)) {
        return errPublicModulus;
    }
    if (pub->E < 2) {
        return errPublicExponentSmall;
    }
    if (pub->E > 1 << 31 - 1) {
        return errPublicExponentLarge;
    }
    return nil;
}
PublicKey PrivateKey::Public()
{
    return &this->PublicKey;
}
std::tuple<moku::slice<uint8_t>, moku::error> PrivateKey::Sign(
        Reader rand, moku::slice<uint8_t> msg, SignerOpts opts)
{
    {
        bool ok{false};
        PSSOptions *pssOpts{std::nullptr};

        std::tie(pssOpts, ok) = moku::try_assert<*PSSOptions>(opts);

        if (ok) {
            return SignPSS(rand, this, pssOpts->Hash, msg, pssOpts);
        }
    }
    return SignPKCS1v15(rand, this, opts.HashFunc(), msg);
}
std::tuple<moku::slice<uint8_t>, moku::error> PrivateKey::Decrypt(
        Reader rand, moku::slice<uint8_t> ciphertext, DecrypterOpts opts)
{
}
