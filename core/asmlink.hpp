#ifndef ASMLINK_HPP
#define ASMLINK_HPP

#include <QSharedPointer>

struct AsmLink
{
    int offset{};
    int length{};
    unsigned long long to{};
};

using AsmLinkPtr = QSharedPointer<AsmLink>;


#endif // ASMLINK_HPP
