int Search(int n, std::function<bool(int)> f)
{
    int i{0};
    int j{0};

    std::tie(i, j) = std::tuple<int, int>(0, n);
    while (i < j) {
        int h{0};

        h = int(unsigned int(i + j) >> 1);
        if (!f(h)) {
            i = h + 1;
        } else {
            j = h;
        }
    }
    return i;
}
int SearchInts(moku::slice<int> a, int x)
{
    return Search(len(a), [=](int i) mutable -> bool { return a[i] >= x; });
}
int SearchFloat64s(moku::slice<double> a, double x)
{
    return Search(len(a), [=](int i) mutable -> bool { return a[i] >= x; });
}
int SearchStrings(moku::slice<std::string> a, std::string x)
{
    return Search(len(a), [=](int i) mutable -> bool { return a[i] >= x; });
}
int IntSlice::_SearchByValue(int x)
{
    return SearchInts(this, x);
}
int Float64Slice::_SearchByValue(double x)
{
    return SearchFloat64s(this, x);
}
int StringSlice::_SearchByValue(std::string x)
{
    return SearchStrings(this, x);
}
int main()
{
    _main();
    return 0;
}
