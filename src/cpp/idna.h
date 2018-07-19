#pragma once
#include "bidirule.h"
#include "fmt.h"
#include "math.h"
#include "norm.h"
#include "strings.h"
#include "utf8.h"
namespace idna {
Option BidiRule();
Profile *Display;
Profile *Lookup;
Option MapForLookup();
Profile *New(moku::slice<Option> o);
} // namespace idna
