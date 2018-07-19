std::string refererForURL(URL *lastReq, URL *newReq)
{
    std::string referer{""};

    if (lastReq->Scheme == "https" && newReq->Scheme == "http") {
        return "";
    }
    referer = lastReq->String();
    if (!moku::is_nil(lastReq->User)) {
        auth = lastReq->User->String() + "@";
        referer = strings::Replace(referer, auth, "", 1);
    }
    return referer;
}
std::tuple<Response *, std::function<bool()>, moku::error> Client::send(
        Request *req, Time deadline)
{
    std::function<bool()> didTimeout{std::nullptr};
    moku::error err{};
    Response *resp{std::nullptr};

    if (!moku::is_nil(this->Jar)) {
        {
            Cookie *cookie{std::nullptr};
            for (cookie : moku::range_value<moku::slice<Cookie *>>(
                         this->Jar.Cookies(req->URL))) {
                req->AddCookie(cookie);
            }
        }
    }
    std::tie(resp, didTimeout, err) = send(req, this->transport(), deadline);
    if (!moku::is_nil(err)) {
        return {nil, didTimeout, err};
    }
    if (!moku::is_nil(this->Jar)) {
        {
            moku::slice<Cookie *> rc{};

            rc = resp->Cookies();

            if (len(rc) > 0) {
                this->Jar.SetCookies(req->URL, rc);
            }
        }
    }
    return {resp, nil, nil};
}
Time Client::deadline()
{
    if (this->Timeout > 0) {
        return time::Now().Add(this->Timeout);
    }
    return time::Time{};
}
RoundTripper Client::transport()
{
    if (!moku::is_nil(this->Transport)) {
        return this->Transport;
    }
    return DefaultTransport;
}
std::tuple<Response *, std::function<bool()>, moku::error> send(Request *ireq,
                                                                RoundTripper rt,
                                                                Time deadline)
{
    std::function<bool()> didTimeout{std::nullptr};
    moku::error err{};
    std::function<void()> forkReq{std::nullptr};
    Request *req{std::nullptr};
    Response *resp{std::nullptr};
    std::function<void()> stopTimer{std::nullptr};

    req = ireq;
    if (moku::is_nil(rt)) {
        req->closeBody();
        return {nil,
                alwaysFalse,
                errors::New("http: no Client.Transport or DefaultTransport")};
    }
    if (moku::is_nil(req->URL)) {
        req->closeBody();
        return {nil, alwaysFalse, errors::New("http: nil Request.URL")};
    }
    if (req->RequestURI != "") {
        req->closeBody();
        return {nil,
                alwaysFalse,
                errors::New("http: Request.RequestURI can't be set in client "
                            "requests.")};
    }
    forkReq = [=]() mutable -> void {
        if (ireq == req) {
            req = new (Request);
            *req = *ireq;
        }
    };
    if (moku::is_nil(req->Header)) {
        forkReq();
        req->Header = make(Header);
    }
    {
        Userinfo *u{std::nullptr};

        u = req->URL->User;

        if (!moku::is_nil(u) && req->Header.Get("Authorization") == "") {
            username = u->Username();
            std::tie(password, _) = u->Password();
            forkReq();
            req->Header = cloneHeader(ireq->Header);
            req->Header.Set("Authorization",
                            "Basic " + basicAuth(username, password));
        }
    }
    if (!deadline.IsZero()) {
        forkReq();
    }
    std::tie(stopTimer, didTimeout) = setRequestCancel(req, rt, deadline);
    std::tie(resp, err) = rt.RoundTrip(req);
    if (!moku::is_nil(err)) {
        stopTimer();
        if (!moku::is_nil(resp)) {
            log::Printf(
                    "RoundTripper returned a response & error; ignoring "
                    "response");
        }
        {
            bool ok{false};
            RecordHeaderError tlsErr{};

            std::tie(tlsErr, ok) =
                    moku::try_assert<tls::RecordHeaderError>(err);

            if (ok) {
                if (std::string(moku::slice_expr<std::vector<uint8_t>>(
                            tlsErr.RecordHeader)) == "HTTP/") {
                    err = errors::New(
                            "http: server gave HTTP response to HTTPS client");
                }
            }
        }
        return {nil, didTimeout, err};
    }
    if (!deadline.IsZero()) {
        resp->Body = &cancelTimerBody{{stop, stopTimer},
                                      {rc, resp->Body},
                                      {reqDidTimeout, didTimeout}};
    }
    return {resp, nil, nil};
}
std::tuple<std::function<void()>, std::function<bool()>> setRequestCancel(
        Request *req, RoundTripper rt, Time deadline)
{
}
