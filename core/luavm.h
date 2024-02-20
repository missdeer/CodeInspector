#ifndef LUAWRAPPER_H
#define LUAWRAPPER_H

#include <lua.hpp>

#include <QStringList>
#include <QtDebug>

class LuaVM
{
public:
    LuaVM();
    ~LuaVM();
    LuaVM(const LuaVM &)            = delete;
    LuaVM &operator=(const LuaVM &) = delete;
    LuaVM(LuaVM &&)                 = delete;
    LuaVM &operator=(LuaVM &&)      = delete;

    void start();
    void shutdown();
    bool doFile(const QString &file);
    bool doScript(const QString &script);

    template<typename T> T get(const QString &name);

    bool get(const QString &name, QStringList &array);
    bool get(const QString &name, double &value);
    bool get(const QString &name, float &value);
    bool get(const QString &name, int &value);
    bool get(const QString &name, long long &value);
    bool get(const QString &name, bool &value);
    bool get(const QString &name, QString &value);

    bool getQStringArray(const QString &name, QStringList &array);

    double getDouble(const QString &name);
    double getDouble(const QString &table, const QString &name);
    double getDouble(const QString &t1, const QString &t2, const QString &name);
    double getDouble(const QString &t1, const QString &t2, const QString &t3, const QString &name);

    float getFloat(const QString &name);
    float getFloat(const QString &table, const QString &name);
    float getFloat(const QString &t1, const QString &t2, const QString &name);
    float getFloat(const QString &t1, const QString &t2, const QString &t3, const QString &name);

    int getInt(const QString &name);
    int getInt(const QString &table, const QString &name);
    int getInt(const QString &t1, const QString &t2, const QString &name);
    int getInt(const QString &t1, const QString &t2, const QString &t3, const QString &name);

    long long getLongLong(const QString &name);
    long long getLongLong(const QString &table, const QString &name);
    long long getLongLong(const QString &t1, const QString &t2, const QString &name);
    long long getLongLong(const QString &t1, const QString &t2, const QString &t3, const QString &name);

    bool getBool(const QString &name);
    bool getBool(const QString &table, const QString &name);
    bool getBool(const QString &t1, const QString &t2, const QString &name);
    bool getBool(const QString &t1, const QString &t2, const QString &t3, const QString &name);

    QString getQString(const QString &name);
    QString getQString(const QString &table, const QString &name);
    QString getQString(const QString &t1, const QString &t2, const QString &name);
    QString getQString(const QString &t1, const QString &t2, const QString &t3, const QString &name);

    bool set(const QString &name, double value);
    bool set(const QString &t1, const QString &name, double value);
    bool set(const QString &t1, const QString &t2, const QString &name, double value);
    bool set(const QString &t1, const QString &t2, const QString &t3, const QString &name, double value);

    bool set(const QString &name, float value);
    bool set(const QString &t1, const QString &name, float value);
    bool set(const QString &t1, const QString &t2, const QString &name, float value);
    bool set(const QString &t1, const QString &t2, const QString &t3, const QString &name, float value);

    bool set(const QString &name, int value);
    bool set(const QString &t1, const QString &name, int value);
    bool set(const QString &t1, const QString &t2, const QString &name, int value);
    bool set(const QString &t1, const QString &t2, const QString &t3, const QString &name, int value);

    bool set(const QString &name, long long value);
    bool set(const QString &t1, const QString &name, long long value);
    bool set(const QString &t1, const QString &t2, const QString &name, long long value);
    bool set(const QString &t1, const QString &t2, const QString &t3, const QString &name, long long value);

    bool set(const QString &name, bool value);
    bool set(const QString &t1, const QString &name, bool value);
    bool set(const QString &t1, const QString &t2, const QString &name, bool value);
    bool set(const QString &t1, const QString &t2, const QString &t3, const QString &name, bool value);

    bool set(const QString &name, const char *value);
    bool set(const QString &t1, const QString &name, const char *value);
    bool set(const QString &t1, const QString &t2, const QString &name, const char *value);
    bool set(const QString &t1, const QString &t2, const QString &t3, const QString &name, const char *value);

    bool doFile(const std::string &file);
    bool doScript(const std::string &script);

    bool getQStringArray(const std::string &name, QStringList &array);

    double getDouble(const std::string &name);
    double getDouble(const std::string &table, const std::string &name);
    double getDouble(const std::string &t1, const std::string &t2, const std::string &name);
    double getDouble(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name);

    float getFloat(const std::string &name);
    float getFloat(const std::string &table, const std::string &name);
    float getFloat(const std::string &t1, const std::string &t2, const std::string &name);
    float getFloat(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name);

    int getInt(const std::string &name);
    int getInt(const std::string &table, const std::string &name);
    int getInt(const std::string &t1, const std::string &t2, const std::string &name);
    int getInt(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name);

    long long getLongLong(const std::string &name);
    long long getLongLong(const std::string &table, const std::string &name);
    long long getLongLong(const std::string &t1, const std::string &t2, const std::string &name);
    long long getLongLong(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name);

    bool getBool(const std::string &name);
    bool getBool(const std::string &table, const std::string &name);
    bool getBool(const std::string &t1, const std::string &t2, const std::string &name);
    bool getBool(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name);

    QString getQString(const std::string &name);
    QString getQString(const std::string &table, const std::string &name);
    QString getQString(const std::string &t1, const std::string &t2, const std::string &name);
    QString getQString(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name);

    std::string getString(const std::string &name);
    std::string getString(const std::string &table, const std::string &name);
    std::string getString(const std::string &t1, const std::string &t2, const std::string &name);
    std::string getString(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name);

    bool set(const std::string &name, double value);
    bool set(const std::string &t1, const std::string &name, double value);
    bool set(const std::string &t1, const std::string &t2, const std::string &name, double value);
    bool set(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name, double value);

    bool set(const std::string &name, float value);
    bool set(const std::string &t1, const std::string &name, float value);
    bool set(const std::string &t1, const std::string &t2, const std::string &name, float value);
    bool set(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name, float value);

    bool set(const std::string &name, int value);
    bool set(const std::string &t1, const std::string &name, int value);
    bool set(const std::string &t1, const std::string &t2, const std::string &name, int value);
    bool set(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name, int value);

    bool set(const std::string &name, long long value);
    bool set(const std::string &t1, const std::string &name, long long value);
    bool set(const std::string &t1, const std::string &t2, const std::string &name, long long value);
    bool set(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name, long long value);

    bool set(const std::string &name, bool value);
    bool set(const std::string &t1, const std::string &name, bool value);
    bool set(const std::string &t1, const std::string &t2, const std::string &name, bool value);
    bool set(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name, bool value);

    bool set(const std::string &name, const char *value);
    bool set(const std::string &t1, const std::string &name, const char *value);
    bool set(const std::string &t1, const std::string &t2, const std::string &name, const char *value);
    bool set(const std::string &t1, const std::string &t2, const std::string &t3, const std::string &name, const char *value);

    bool doFile(const char *file);
    bool doScript(const char *script, size_t len);

    bool getQStringArray(const char *name, QStringList &array);

    double getDouble(const char *name);
    double getDouble(const char *table, const char *name);
    double getDouble(const char *t1, const char *t2, const char *name);
    double getDouble(const char *t1, const char *t2, const char *t3, const char *name);

    float getFloat(const char *name);
    float getFloat(const char *table, const char *name);
    float getFloat(const char *t1, const char *t2, const char *name);
    float getFloat(const char *t1, const char *t2, const char *t3, const char *name);

    int getInt(const char *name);
    int getInt(const char *table, const char *name);
    int getInt(const char *t1, const char *t2, const char *name);
    int getInt(const char *t1, const char *t2, const char *t3, const char *name);

    long long getLongLong(const char *name);
    long long getLongLong(const char *table, const char *name);
    long long getLongLong(const char *t1, const char *t2, const char *name);
    long long getLongLong(const char *t1, const char *t2, const char *t3, const char *name);

    bool getBool(const char *name);
    bool getBool(const char *table, const char *name);
    bool getBool(const char *t1, const char *t2, const char *name);
    bool getBool(const char *t1, const char *t2, const char *t3, const char *name);

    QString getQString(const char *name);
    QString getQString(const char *table, const char *name);
    QString getQString(const char *t1, const char *t2, const char *name);
    QString getQString(const char *t1, const char *t2, const char *t3, const char *name);

    std::string getString(const char *name);
    std::string getString(const char *table, const char *name);
    std::string getString(const char *t1, const char *t2, const char *name);
    std::string getString(const char *t1, const char *t2, const char *t3, const char *name);

    bool set(const char *name, double value);
    bool set(const char *t1, const char *name, double value);
    bool set(const char *t1, const char *t2, const char *name, double value);
    bool set(const char *t1, const char *t2, const char *t3, const char *name, double value);

    bool set(const char *name, float value);
    bool set(const char *t1, const char *name, float value);
    bool set(const char *t1, const char *t2, const char *name, float value);
    bool set(const char *t1, const char *t2, const char *t3, const char *name, float value);

    bool set(const char *name, int value);
    bool set(const char *t1, const char *name, int value);
    bool set(const char *t1, const char *t2, const char *name, int value);
    bool set(const char *t1, const char *t2, const char *t3, const char *name, int value);

    bool set(const char *name, long long value);
    bool set(const char *t1, const char *name, long long value);
    bool set(const char *t1, const char *t2, const char *name, long long value);
    bool set(const char *t1, const char *t2, const char *t3, const char *name, long long value);

    bool set(const char *name, bool value);
    bool set(const char *t1, const char *name, bool value);
    bool set(const char *t1, const char *t2, const char *name, bool value);
    bool set(const char *t1, const char *t2, const char *t3, const char *name, bool value);

    bool set(const char *name, const char *value);
    bool set(const char *t1, const char *name, const char *value);
    bool set(const char *t1, const char *t2, const char *name, const char *value);
    bool set(const char *t1, const char *t2, const char *t3, const char *name, const char *value);

private:
    lua_State *m_L {nullptr};

    void loadBuiltinTemplate();
    void setupBuiltinValues();
};

template<> QStringList LuaVM::get(const QString &name)
{
    QStringList res;
    if (!m_L)
    {
        return res;
    }

    QStringList sections = name.split(".");
    if (sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qCritical() << "empty name";
#endif
        return res;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qCritical() << sections[i] << " is expected to be a table";
#endif
            lua_pop(m_L, i + 1);
            return res;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_istable(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qCritical() << name << " is expected to be a table";
#endif
        lua_pop(m_L, sections.length());
        return res;
    }

    size_t n = lua_rawlen(m_L, -1);

    for (size_t i = 1; i <= n; i++)
    {
        lua_rawgeti(m_L, -1, i);

        size_t      resultLen = 0;
        const char *result    = lua_tolstring(m_L, -1, &resultLen);
        QString     str       = QString::fromUtf8(result, (int)resultLen);
        lua_pop(m_L, 1);
        res.append(str);
    }

    return res;
}

template<> double LuaVM::get(const QString &name)
{
    if (!m_L)
    {
        return 0.0;
    }

    QStringList sections = name.split(".");
    if (sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qCritical() << "empty name";
#endif
        return 0.0;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qCritical() << sections[i] << " is expected to be a table";
#endif
            lua_pop(m_L, i + 1);
            return 0.0;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_isnumber(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qCritical() << name << " is expected to be a number";
#endif
        lua_pop(m_L, sections.length());
        return 0.0;
    }

    double result = lua_tonumber(m_L, -1);
    lua_pop(m_L, sections.length());

    return result;
}

template<> float LuaVM::get(const QString &name)
{
    if (!m_L)
    {
        return 0.0;
    }

    QStringList sections = name.split(".");
    if (sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qCritical() << "empty name";
#endif
        return 0.0;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qCritical() << sections[i] << " is expected to be a table";
#endif
            lua_pop(m_L, i + 1);
            return 0.0;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_isnumber(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qCritical() << name << " is expected to be a number";
#endif
        lua_pop(m_L, sections.length());

        return 0.0;
    }

    float result = (float)lua_tonumber(m_L, -1);
    lua_pop(m_L, sections.length());

    return result;
}

template<> int LuaVM::get(const QString &name)
{
    if (!m_L)
    {
        return 0;
    }

    QStringList sections = name.split(".");
    if (sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qCritical() << "empty name";
#endif
        return 0;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qCritical() << sections[i] << " is expected to be a table";
#endif
            lua_pop(m_L, i + 1);
            return 0;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_isinteger(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qCritical() << name << " is expected to be an integer";
#endif
        lua_pop(m_L, sections.length());
        return 0;
    }

    int result = (int)lua_tointeger(m_L, -1);
    lua_pop(m_L, sections.length());

    return result;
}

template<> long long LuaVM::get(const QString &name)
{
    if (!m_L)
    {
        return 0;
    }

    QStringList sections = name.split(".");
    if (sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qCritical() << "empty name";
#endif
        return 0;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qCritical() << sections[i] << " is expected to be a table";
#endif
            lua_pop(m_L, i + 1);
            return 0.0;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_isinteger(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qCritical() << name << " is expected to be an integer";
#endif
        lua_pop(m_L, sections.length());
        return 0.0;
    }

    long long result = (long long)lua_tointeger(m_L, -1);
    lua_pop(m_L, sections.length());

    return result;
}

template<> bool LuaVM::get(const QString &name)
{
    if (!m_L)
    {
        return false;
    }

    QStringList sections = name.split(".");
    if (sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qCritical() << "empty name";
#endif
        return false;
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qCritical() << sections[i] << " is expected to be a table";
#endif
            lua_pop(m_L, i + 1);
            return false;
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_isboolean(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qCritical() << name << " is expected to be a boolean";
#endif
        lua_pop(m_L, sections.length());
        return false;
    }

    int result = lua_toboolean(m_L, -1);
    lua_pop(m_L, sections.length());

    return result;
}

template<> QString LuaVM::get(const QString &name)
{
    if (!m_L)
    {
        return {};
    }

    QStringList sections = name.split(".");
    if (sections.isEmpty())
    {
#if defined(LOGS_ENABLED)
        qCritical() << "empty name";
#endif
        return {};
    }
    lua_getglobal(m_L, sections[0].toStdString().c_str());
    for (int i = 0; i < sections.length() - 1; i++)
    {
        if (!lua_istable(m_L, -1))
        {
#if defined(LOGS_ENABLED)
            qCritical() << sections[i] << " is expected to be a table";
#endif
            lua_pop(m_L, i + 1);
            return {};
        }

        lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
        lua_gettable(m_L, -2); /* get t1[t2] */
    }

    if (!lua_isstring(m_L, -1))
    {
#if defined(LOGS_ENABLED)
        qCritical() << name << " is expected to be a string";
#endif
        lua_pop(m_L, sections.length());
        return {};
    }

    size_t      resultLen = 0;
    const char *result    = lua_tolstring(m_L, -1, &resultLen);
    QString     str       = QString::fromUtf8(result, (int)resultLen);
    lua_pop(m_L, sections.length());

    return str;
}

#endif // LUAWRAPPER_H
