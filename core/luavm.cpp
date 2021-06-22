#include "stdafx.h"
#include "luavm.h"

LuaVM::LuaVM()
{
    start();
}

LuaVM::~LuaVM()
{
    shutdown();
}

void LuaVM::start()
{
    if (!m_L)
    {
        m_L = luaL_newstate();
    
        luaL_openlibs(m_L);
    
        loadBuiltinTemplate();
    
        setupBuiltinValues();        
    }
}

void LuaVM::shutdown()
{
    if (m_L)
    {
        lua_close(m_L);
        m_L = nullptr;
    }    
}

bool LuaVM::doFile(const QString &file)
{
    if (!m_L)
        return false;

    int status = luaL_loadfile(m_L, file.toStdString().c_str());
    if (status)
    {
#if defined(LOGS_ENABLED)
        qDebug() << "Couldn't load file:" << lua_tostring(m_L, -1);
#endif
        lua_pop(m_L, 1);
        return false;
    }

    status = lua_pcall(m_L, 0, 0, 0);
    if (status)
    {
#if defined(LOGS_ENABLED)
        qDebug() << "Couldn't run file:" << lua_tostring(m_L, -1);
#endif
        lua_pop(m_L, 1);
        return false;
    }
    return true;
}

bool LuaVM::doScript(const QString &script)
{
    if (!m_L)
        return false;
    int status = luaL_loadbuffer(m_L, script.toStdString().c_str(), script.toStdString().size(), "theme");
    if (status)
    {
#if defined(LOGS_ENABLED)
        qDebug() << "Couldn't load script:" << lua_tostring(m_L, -1);
#endif
        lua_pop(m_L, 1);
        return false;
    }

    status = lua_pcall(m_L, 0, 0, 0);
    if (status)
    {
#if defined(LOGS_ENABLED)
        qDebug() << "Couldn't run script:" << lua_tostring(m_L, -1);
#endif
        lua_pop(m_L, 1);
        return false;
    }
    return true;
}

bool LuaVM::get(const QString &name, QStringList &array)
{
    if (!m_L)
        return false;

    QStringList sections = name.split(".");
    if (!sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "empty name";
#endif
        return false;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << sections[i] << " is expected to be a table";
#endif
            return false;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_istable(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qDebug() << name << " is expected to be a table";
#endif
        return false;
    }
    
    size_t n = lua_rawlen(m_L, -1);
    
    for (size_t i = 1; i <= n; i++)
    {
        lua_rawgeti(m_L, -1, i);
        
        size_t resultLen = 0;
        const char * result = lua_tolstring(m_L, -1, &resultLen);
        QString str = QString::fromLatin1(result, (int)resultLen);
        lua_pop(m_L, 1);
        array.append(str);
    }
    
    return true;
}

bool LuaVM::get(const QString &name, double &value)
{
    if (!m_L)
        return false;

    QStringList sections = name.split(".");
    if (!sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "empty name";
#endif
        return false;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << sections[i] << " is expected to be a table";
#endif
            return false;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_isnumber(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qDebug() << name << " is expected to be a number";
#endif
        return false;
    }

    value = lua_tonumber(m_L, -1);
    lua_pop(m_L, sections.length());

    return true;
}

bool LuaVM::get(const QString &name, float &value)
{
    if (!m_L)
        return false;

    QStringList sections = name.split(".");
    if (!sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "empty name";
#endif
        return false;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << sections[i] << " is expected to be a table";
#endif
            return false;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_isnumber(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qDebug() << name << " is expected to be a number";
#endif
        return false;
    }

    value = (float)lua_tonumber(m_L, -1);
    lua_pop(m_L, sections.length());

    return true;
}

bool LuaVM::get(const QString &name, int &value)
{
    if (!m_L)
        return false;

    QStringList sections = name.split(".");
    if (!sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "empty name";
#endif
        return false;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << sections[i] << " is expected to be a table";
#endif
            return false;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_isinteger(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qDebug() << name << " is expected to be an integer";
#endif
        return false;
    }

    value = (int)lua_tointeger(m_L, -1);
    lua_pop(m_L, sections.length());

    return true;
}

bool LuaVM::get(const QString &name, long long &value)
{
    if (!m_L)
        return false;

    QStringList sections = name.split(".");
    if (!sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "empty name";
#endif
        return false;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << sections[i] << " is expected to be a table";
#endif
            return false;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_isinteger(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qDebug() << name << " is expected to be an integer";
#endif
        return false;
    }

    value = (long long)lua_tointeger(m_L, -1);
    lua_pop(m_L, sections.length());

    return true;
}

bool LuaVM::get(const QString &name, bool &value)
{
    if (!m_L)
        return false;

    QStringList sections = name.split(".");
    if (!sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "empty name";
#endif
        return false;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << sections[i] << " is expected to be a table";
#endif
            return 0.0;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_isboolean(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qDebug() << name << " is expected to be a boolean";
#endif
        return false;
    }

    value = lua_toboolean(m_L, -1);
    lua_pop(m_L, sections.length());

    return true;
}

bool LuaVM::get(const QString &name, QString &value)
{
    if (!m_L)
        return false;

    QStringList sections = name.split(".");
    if (!sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "empty name";
#endif
        return false;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << sections[i] << " is expected to be a table";
#endif
            return false;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_isstring(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qDebug() << name << " is expected to be a string";
#endif
        return false;
    }

    size_t      resultLen = 0;
    const char *result    = lua_tolstring(m_L, -1, &resultLen);
    value                 = QString::fromLatin1(result, (int)resultLen);
    lua_pop(m_L, sections.length());

    return true;
}

bool LuaVM::set(const QString &name, double value)
{
    if (!m_L)
        return false;

    QStringList sections = name.split(".");
    if (!sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "empty name";
#endif
        return false;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());

    if (sections.length() == 1)
    {
        if (!lua_isnumber(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << name << " is expected to be a number";
#endif
            return false;
        }

        lua_pushnumber(m_L, value);
        lua_setglobal(m_L, sections[0].toStdString().c_str());
        return true;
    }

    for (int i = 0; i < sections.length() - 2; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << sections[i] << " is expected to be a table";
#endif
            return false;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_istable(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qDebug() << sections[sections.length() - 2] << " is expected to be a table";
#endif
        return false;
    }

    lua_pushnumber(m_L, value);
    lua_setfield(m_L, -2, sections[sections.length() - 1].toStdString().c_str());

    lua_pop(m_L, sections.length() - 1);
    return true;
}

bool LuaVM::set(const QString &name, float value)
{
    return set(name, (double)value);
}

bool LuaVM::set(const QString &name, long long value)
{
    if (!m_L)
        return false;

    QStringList sections = name.split(".");
    if (!sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "empty name";
#endif
        return false;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());

    if (sections.length() == 1)
    {
        if (!lua_isnumber(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << name << " is expected to be a number";
#endif
            return false;
        }

        lua_pushinteger(m_L, value);
        lua_setglobal(m_L, sections[0].toStdString().c_str());
        return true;
    }
    for (int i = 0; i < sections.length() - 2; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << sections[i] << " is expected to be a table";
#endif
            return false;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_istable(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qDebug() << sections[sections.length() - 2] << " is expected to be a table";
#endif
        return false;
    }

    lua_pushinteger(m_L, value);
    lua_setfield(m_L, -2, sections[sections.length() - 1].toStdString().c_str());

    lua_pop(m_L, sections.length() - 1);
    return true;
}

bool LuaVM::set(const QString &name, int value)
{
    return set(name, (long long)value);
}

bool LuaVM::set(const QString &name, bool value)
{
    QStringList sections = name.split(".");
    if (!sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "empty name";
#endif
        return false;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());

    if (sections.length() == 1)
    {
        if (!lua_isnumber(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << name << " is expected to be a number";
#endif
            return false;
        }

        lua_pushboolean(m_L, value);
        lua_setglobal(m_L, sections[0].toStdString().c_str());
        return true;
    }
    for (int i = 0; i < sections.length() - 2; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << sections[i] << " is expected to be a table";
#endif
            return false;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_istable(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qDebug() << sections[sections.length() - 2] << " is expected to be a table";
#endif
        return false;
    }

    lua_pushboolean(m_L, value);
    lua_setfield(m_L, -2, sections[sections.length() - 1].toStdString().c_str());

    lua_pop(m_L, sections.length() - 1);
    return true;
}

bool LuaVM::set(const QString &name, const char *value)
{
    QStringList sections = name.split(".");
    if (!sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "empty name";
#endif
        return false;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());

    if (sections.length() == 1)
    {
        if (!lua_isnumber(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << name << " is expected to be a number";
#endif
            return false;
        }

        lua_pushstring(m_L, value);
        lua_setglobal(m_L, sections[0].toStdString().c_str());
        return true;
    }

    for (int i = 0; i < sections.length() - 2; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qDebug() << sections[i] << " is expected to be a table";
#endif
            return false;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_istable(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qDebug() << sections[sections.length() - 2] << " is expected to be a table";
#endif
        return false;
    }

    lua_pushstring(m_L, value);
    lua_setfield(m_L, -2, sections[sections.length() - 1].toStdString().c_str());

    lua_pop(m_L, sections.length() - 1);
    return true;
}

bool LuaVM::set(const QString &name, const QString &value)
{
    return set(name, value.toStdString().c_str());
}

void LuaVM::loadBuiltinTemplate()
{
    QFile f(":/lua/builtin.lua");
    if (f.open(QIODevice::ReadOnly))
    {
        QByteArray ba = f.readAll();
        f.close();

        doScript(ba);
    }
}

void LuaVM::setupBuiltinValues()
{
#if defined(Q_OS_MAC)
    set("os", "macos");
    set("macOS", true);
#elif defined(Q_OS_WIN)
    set("os", "windows");
    set("Windows", true);
#elif defined(Q_OS_IOS)
    set("os", "ios");
    set("iOS", true);
#elif defined(Q_OS_ANDROID)
    set("os", "android");
    set("Android", true);
#elif defined(Q_OS_LINUX)
    set("os", "linux");
    set("Linux", true);
#else
    set("os", "other");
#endif
}
