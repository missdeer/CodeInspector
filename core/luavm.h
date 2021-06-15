#ifndef LUAWRAPPER_H
#define LUAWRAPPER_H

#include <lua.hpp>
#include <QStringList>

class LuaVM
{
public:
    LuaVM();
    ~LuaVM();
    LuaVM(const LuaVM&) = delete;
    LuaVM& operator=(const LuaVM&) = delete;
    LuaVM(LuaVM &&) = delete;
    LuaVM& operator=(LuaVM &&) = delete;

    void start();
    void shutdown();
    bool doFile(const QString &file);
    bool doScript(const QString &script);

    template<typename T> T get(const QString &name);
    template<> QStringList get(const QString &name)
    {
        QStringList res;
        if (!m_L)
            return res;

        QStringList sections = name.split(".");
        if (!sections.isEmpty())
        {
            qDebug() << "empty name";
            return res;
        }
        lua_getglobal(m_L, sections[0].toStdString().c_str());
        for (int i = 0; i < sections.length() - 1; i++)
        {
            if (!lua_istable(m_L, -1))
            {
                qDebug() << sections[i] << " is expected to be a table";
                return res;
            }

            lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
            lua_gettable(m_L, -2); /* get t1[t2] */
        }

        if (!lua_istable(m_L, -1))
        {
            qDebug() << name << " is expected to be a table";
            return res;
        }

        size_t n = lua_rawlen(m_L, -1);

        for (size_t i = 1; i <= n; i++)
        {
            lua_rawgeti(m_L, -1, i);

            size_t      resultLen = 0;
            const char *result    = lua_tolstring(m_L, -1, &resultLen);
            QString     str       = QString::fromLatin1(result, (int)resultLen);
            lua_pop(m_L, 1);
            res.append(str);
        }

        return res;
    }
    template<> double get(const QString &name)
    {
        if (!m_L)
            return 0.0;

        QStringList sections = name.split(".");
        if (!sections.isEmpty())
        {
            qDebug() << "empty name";
            return 0.0;
        }
        lua_getglobal(m_L, sections[0].toStdString().c_str());
        for (int i = 0; i < sections.length() - 1; i++)
        {
            if (!lua_istable(m_L, -1))
            {
                qDebug() << sections[i] << " is expected to be a table";
                return 0.0;
            }

            lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
            lua_gettable(m_L, -2); /* get t1[t2] */
        }

        if (!lua_isnumber(m_L, -1))
        {
            qDebug() << name << " is expected to be a number";
            return 0.0;
        }

        double result = lua_tonumber(m_L, -1);
        lua_pop(m_L, sections.length());

        return result;
    }
    template<> float get(const QString &name)
    {
        if (!m_L)
            return 0.0;

        QStringList sections = name.split(".");
        if (!sections.isEmpty())
        {
            qDebug() << "empty name";
            return 0.0;
        }
        lua_getglobal(m_L, sections[0].toStdString().c_str());
        for (int i = 0; i < sections.length() - 1; i++)
        {
            if (!lua_istable(m_L, -1))
            {
                qDebug() << sections[i] << " is expected to be a table";
                return 0.0;
            }

            lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
            lua_gettable(m_L, -2); /* get t1[t2] */
        }

        if (!lua_isnumber(m_L, -1))
        {
            qDebug() << name << " is expected to be a number";
            return 0.0;
        }

        float result = (float)lua_tonumber(m_L, -1);
        lua_pop(m_L, sections.length());

        return result;
    }
    template<> int get(const QString &name)
    {
        if (!m_L)
            return 0;

        QStringList sections = name.split(".");
        if (!sections.isEmpty())
        {
            qDebug() << "empty name";
            return 0;
        }
        lua_getglobal(m_L, sections[0].toStdString().c_str());
        for (int i = 0; i < sections.length() - 1; i++)
        {
            if (!lua_istable(m_L, -1))
            {
                qDebug() << sections[i] << " is expected to be a table";
                return 0;
            }

            lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
            lua_gettable(m_L, -2); /* get t1[t2] */
        }

        if (!lua_isinteger(m_L, -1))
        {
            qDebug() << name << " is expected to be an integer";
            return 0;
        }

        int result = (int)lua_tointeger(m_L, -1);
        lua_pop(m_L, sections.length());

        return result;
    }
    template<> long long get(const QString &name)
    {
        if (!m_L)
            return 0.0;

        QStringList sections = name.split(".");
        if (!sections.isEmpty())
        {
            qDebug() << "empty name";
            return 0;
        }
        lua_getglobal(m_L, sections[0].toStdString().c_str());
        for (int i = 0; i < sections.length() - 1; i++)
        {
            if (!lua_istable(m_L, -1))
            {
                qDebug() << sections[i] << " is expected to be a table";
                return 0.0;
            }

            lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
            lua_gettable(m_L, -2); /* get t1[t2] */
        }

        if (!lua_isinteger(m_L, -1))
        {
            qDebug() << name << " is expected to be an integer";
            return 0.0;
        }

        long long result = (long long)lua_tointeger(m_L, -1);
        lua_pop(m_L, sections.length());

        return result;
    }

    template<> bool get(const QString &name)
    {
        if (!m_L)
            return false;

        QStringList sections = name.split(".");
        if (!sections.isEmpty())
        {
            qDebug() << "empty name";
            return false;
        }
        lua_getglobal(m_L, sections[0].toStdString().c_str());
        for (int i = 0; i < sections.length() - 1; i++)
        {
            if (!lua_istable(m_L, -1))
            {
                qDebug() << sections[i] << " is expected to be a table";
                return false;
            }

            lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
            lua_gettable(m_L, -2); /* get t1[t2] */
        }

        if (!lua_isboolean(m_L, -1))
        {
            qDebug() << name << " is expected to be a boolean";
            return false;
        }

        int result = lua_toboolean(m_L, -1);
        lua_pop(m_L, sections.length());

        return result;
    }

    template<> QString get(const QString &name)
    {
        if (!m_L)
            return "";

        QStringList sections = name.split(".");
        if (!sections.isEmpty())
        {
            qDebug() << "empty name";
            return "";
        }
        lua_getglobal(m_L, sections[0].toStdString().c_str());
        for (int i = 0; i < sections.length() - 1; i++)
        {
            if (!lua_istable(m_L, -1))
            {
                qDebug() << sections[i] << " is expected to be a table";
                return "";
            }

            lua_pushstring(m_L, sections[i + 1].toStdString().c_str());
            lua_gettable(m_L, -2); /* get t1[t2] */
        }

        if (!lua_isstring(m_L, -1))
        {
            qDebug() << name << " is expected to be a string";
            return "";
        }

        size_t      resultLen = 0;
        const char *result    = lua_tolstring(m_L, -1, &resultLen);
        QString     str       = QString::fromLatin1(result, (int)resultLen);
        lua_pop(m_L, sections.length());

        return str;
    }

    bool get(const QString &name, QStringList &array);
    bool get(const QString &name, double &value);
    bool get(const QString &name, float &value);
    bool get(const QString &name, int &value);
    bool get(const QString &name, long long &value);
    bool get(const QString &name, bool &value);
    bool get(const QString &name, QString &value);

    bool set(const QString &name, double value);
    bool set(const QString &name, float value);
    bool set(const QString &name, int value);   
    bool set(const QString &name, long long value);  
    bool set(const QString &name, bool value); 
    bool set(const QString &name, const char *value);
    bool set(const QString &name, const QString &value);

private:
    lua_State *m_L {nullptr};

    void loadBuiltinTemplate();
    void setupBuiltinValues();
};

#endif // LUAWRAPPER_H
