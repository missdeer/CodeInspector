#include "stdafx.h"

#include "scopedguard.h"

#include <utility>

ScopedGuard::ScopedGuard(std::function<void(void)> f) : m_f(std::move(f)) {}

ScopedGuard::~ScopedGuard()
{
    m_f();
}
