#pragma once
namespace nettrace {

struct LookupIPAltResolverKey {
};
template <>
inline bool moku::is_nil<LookupIPAltResolverKey>(
        const LookupIPAltResolverKey &lookupipaltresolverkey)
{
    return;
}
struct Trace {
    std::function<void(std::string)> DNSStart{std::nullptr};
    std::function<void(moku::slice<moku::interface>, bool, moku::error)>
            DNSDone{std::nullptr};
    std::function<void(std::string, std::string)> ConnectStart{std::nullptr};
    std::function<void(std::string, std::string, moku::error)> ConnectDone{
            std::nullptr};
};
template <>
inline bool moku::is_nil<Trace>(const Trace &trace)
{
    return moku::is_nil<std::function<void(std::string)>>(trace.DNSStart) &&
           moku::is_nil<std::function<void(
                   moku::slice<moku::interface>, bool, moku::error)>>(
                   trace.DNSDone) &&
           moku::is_nil<std::function<void(std::string, std::string)>>(
                   trace.ConnectStart) &&
           moku::is_nil<
                   std::function<void(std::string, std::string, moku::error)>>(
                   trace.ConnectDone);
}
struct TraceKey {
};
template <>
inline bool moku::is_nil<TraceKey>(const TraceKey &tracekey)
{
    return;
}
} // namespace nettrace
