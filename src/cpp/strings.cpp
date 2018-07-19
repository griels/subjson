int Compare(std::string a, std::string b)
{
    if (a == b) {
        return 0;
    }
    if (a < b) {
        return -1;
    }
    return +1;
}
void init();
int main()
{
    init();
    asciiSpace = std::vector<uint8_t>{
            {'\t', 1}, {'\n', 1}, {'\v', 1}, {'\f', 1}, {'\r', 1}, {' ', 1}};
    _main();
    return 0;
}
