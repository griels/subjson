CertPool *NewCertPool()
{
    return &CertPool{
            {bySubjectKeyId, make(std::map<std::string, moku::slice<int>>)},
            {byName, make(std::map<std::string, moku::slice<int>>)}};
}
std::tuple<CertPool *, moku::error> SystemCertPool()
{
    if (runtime::GOOS == "windows") {
        return {nil,
                errors::New("crypto/x509: system root pool is not available on "
                            "Windows")};
    }
    return loadSystemRoots();
}
std::tuple<moku::slice<int>, Certificate *, moku::error>
CertPool::findVerifiedParents(Certificate *cert)
{
    moku::slice<int> candidates{};
    moku::error err{};
    Certificate *errCert{std::nullptr};
    moku::slice<int> parents{};

    if (moku::is_nil(this)) {
        return;
    }
    if (len(cert->AuthorityKeyId) > 0) {
        candidates = this->bySubjectKeyId[std::string(cert->AuthorityKeyId)];
    }
    if (len(candidates) == 0) {
        candidates = this->byName[std::string(cert->RawIssuer)];
    }
    {
        int c{0};
        for (c : moku::range_value<moku::slice<int>>(candidates)) {
            {
                err = cert->CheckSignatureFrom(this->certs[c]);

                if (moku::is_nil(err)) {
                    parents = append(parents, c);
                } else {
                    errCert = this->certs[c];
                }
            }
        }
    }
    return;
}
bool CertPool::contains(Certificate *cert)
{
    moku::slice<int> candidates{};

    if (moku::is_nil(this)) {
        return false;
    }
    candidates = this->byName[std::string(cert->RawSubject)];
    {
        int c{0};
        for (c : moku::range_value<moku::slice<int>>(candidates)) {
            if (this->certs[c]->Equal(cert)) {
                return true;
            }
        }
    }
    return false;
}
void CertPool::AddCert(Certificate *cert)
{
    int n{0};
    std::string name{""};

    if (moku::is_nil(cert)) {
        panic(moku::make_iface<std::string>(
                &{ % !s(token.Pos = 4966498) STRING
                     "adding nil Certificate to CertPool"}));
    }
    if (this->contains(cert)) {
        return;
    }
    n = len(this->certs);
    this->certs = append(this->certs, cert);
    if (len(cert->SubjectKeyId) > 0) {
        keyId = std::string(cert->SubjectKeyId);
        this->bySubjectKeyId[keyId] = append(this->bySubjectKeyId[keyId], n);
    }
    name = std::string(cert->RawSubject);
    this->byName[name] = append(this->byName[name], n);
}
bool CertPool::AppendCertsFromPEM(moku::slice<uint8_t> pemCerts)
{
    bool ok{false};

    while (len(pemCerts) > 0) {
        Block *block{std::nullptr};
        Certificate *cert{std::nullptr};
        moku::error err{};

        std::tie(block, pemCerts) = pem::Decode(pemCerts);
        if (moku::is_nil(block)) {
            break;
        }
        if (block->Type != "CERTIFICATE" || len(block->Headers) != 0) {
            continue;
        }
        std::tie(cert, err) = ParseCertificate(block->Bytes);
        if (!moku::is_nil(err)) {
            continue;
        }
        this->AddCert(cert);
        ok = true;
    }
    return;
}
moku::slice<moku::slice<uint8_t>> CertPool::Subjects()
{
    moku::slice<moku::slice<uint8_t>> res{};

    res = make(moku::slice<moku::slice<byte>>, len(this->certs));
    {
        Certificate *c{std::nullptr};
        int i{0};
        for (std::tie(i, c) :
             moku::range_key_value<moku::slice<Certificate *>>(this->certs)) {
            res[i] = c->RawSubject;
        }
    }
    return res;
}
int main()
{
    rfc1423Algos =
            moku::slice<rfc1423Algo>{{{cipher, PEMCipherDES},
                                      {name, "DES-CBC"},
                                      {cipherFunc, des::NewCipher},
                                      {keySize, 8},
                                      {blockSize, des::BlockSize}},
                                     {{cipher, PEMCipher3DES},
                                      {name, "DES-EDE3-CBC"},
                                      {cipherFunc, des::NewTripleDESCipher},
                                      {keySize, 24},
                                      {blockSize, des::BlockSize}},
                                     {{cipher, PEMCipherAES128},
                                      {name, "AES-128-CBC"},
                                      {cipherFunc, aes::NewCipher},
                                      {keySize, 16},
                                      {blockSize, aes::BlockSize}},
                                     {{cipher, PEMCipherAES192},
                                      {name, "AES-192-CBC"},
                                      {cipherFunc, aes::NewCipher},
                                      {keySize, 24},
                                      {blockSize, aes::BlockSize}},
                                     {{cipher, PEMCipherAES256},
                                      {name, "AES-256-CBC"},
                                      {cipherFunc, aes::NewCipher},
                                      {keySize, 32},
                                      {blockSize, aes::BlockSize}}};
    IncorrectPasswordError = errors::New("x509: decryption password incorrect");
    debugExecDarwinRoots =
            strings::Contains(os::Getenv("GODEBUG"), "x509roots=1");
    errNotParsed =
            errors::New("x509: missing ASN.1 contents; use ParseCertificate");
    algoName = std::vector<std::string>{{MD2WithRSA, "MD2-RSA"},
                                        {MD5WithRSA, "MD5-RSA"},
                                        {SHA1WithRSA, "SHA1-RSA"},
                                        {SHA256WithRSA, "SHA256-RSA"},
                                        {SHA384WithRSA, "SHA384-RSA"},
                                        {SHA512WithRSA, "SHA512-RSA"},
                                        {SHA256WithRSAPSS, "SHA256-RSAPSS"},
                                        {SHA384WithRSAPSS, "SHA384-RSAPSS"},
                                        {SHA512WithRSAPSS, "SHA512-RSAPSS"},
                                        {DSAWithSHA1, "DSA-SHA1"},
                                        {DSAWithSHA256, "DSA-SHA256"},
                                        {ECDSAWithSHA1, "ECDSA-SHA1"},
                                        {ECDSAWithSHA256, "ECDSA-SHA256"},
                                        {ECDSAWithSHA384, "ECDSA-SHA384"},
                                        {ECDSAWithSHA512, "ECDSA-SHA512"}};
    oidSignatureMD2WithRSA = asn1::ObjectIdentifier{1, 2, 840, 113549, 1, 1, 2};
    oidSignatureMD5WithRSA = asn1::ObjectIdentifier{1, 2, 840, 113549, 1, 1, 4};
    oidSignatureSHA1WithRSA =
            asn1::ObjectIdentifier{1, 2, 840, 113549, 1, 1, 5};
    oidSignatureSHA256WithRSA =
            asn1::ObjectIdentifier{1, 2, 840, 113549, 1, 1, 11};
    oidSignatureSHA384WithRSA =
            asn1::ObjectIdentifier{1, 2, 840, 113549, 1, 1, 12};
    oidSignatureSHA512WithRSA =
            asn1::ObjectIdentifier{1, 2, 840, 113549, 1, 1, 13};
    oidSignatureRSAPSS = asn1::ObjectIdentifier{1, 2, 840, 113549, 1, 1, 10};
    oidSignatureDSAWithSHA1 = asn1::ObjectIdentifier{1, 2, 840, 10040, 4, 3};
    oidSignatureDSAWithSHA256 =
            asn1::ObjectIdentifier{2, 16, 840, 1, 101, 3, 4, 3, 2};
    oidSignatureECDSAWithSHA1 = asn1::ObjectIdentifier{1, 2, 840, 10045, 4, 1};
    oidSignatureECDSAWithSHA256 =
            asn1::ObjectIdentifier{1, 2, 840, 10045, 4, 3, 2};
    oidSignatureECDSAWithSHA384 =
            asn1::ObjectIdentifier{1, 2, 840, 10045, 4, 3, 3};
    oidSignatureECDSAWithSHA512 =
            asn1::ObjectIdentifier{1, 2, 840, 10045, 4, 3, 4};
    oidSHA256 = asn1::ObjectIdentifier{2, 16, 840, 1, 101, 3, 4, 2, 1};
    oidSHA384 = asn1::ObjectIdentifier{2, 16, 840, 1, 101, 3, 4, 2, 2};
    oidSHA512 = asn1::ObjectIdentifier{2, 16, 840, 1, 101, 3, 4, 2, 3};
    oidMGF1 = asn1::ObjectIdentifier{1, 2, 840, 113549, 1, 1, 8};
    oidISOSignatureSHA1WithRSA = asn1::ObjectIdentifier{1, 3, 14, 3, 2, 29};
    signatureAlgorithmDetails