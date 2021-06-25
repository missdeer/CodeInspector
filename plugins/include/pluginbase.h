#ifndef PLUGINBASE_H
#define PLUGINBASE_H

#include <QJsonArray>
#include <QJsonObject>

namespace PluginBase
{
    bool hasToolResult(const QJsonObject &docObj, const QString &name, QString &stdOut, QString &stdErr)
    {
        QJsonValue toolsVal = docObj["tools"];
        if (!toolsVal.isArray())
            return false;

        auto toolsArray = toolsVal.toArray();
        auto it         = std::find_if(toolsArray.begin(), toolsArray.end(), [&name](QJsonValueRef v) {
            if (!v.isObject())
                return false;

            auto o = v.toObject();
            if (o.contains("id") && o["id"].isString())
                return o["id"].toString() == name;

            return false;
        });
        if (toolsArray.end() == it)
            return false;

        auto       toolObj   = it->toObject();
        QJsonValue stderrVal = toolObj["stderr"];
        if (stderrVal.isArray())
        {
            QJsonArray stderrArray = stderrVal.toArray();
            for (auto e : stderrArray)
            {
                if (e.isObject())
                {
                    QJsonObject errObj  = e.toObject();
                    QJsonValue  textVal = errObj["text"];
                    if (textVal.isString())
                    {
                        stdErr.append(textVal.toString() + "\n");
                    }
                }
            }
        }
        QJsonValue stdoutVal = toolObj["stdout"];
        if (stdoutVal.isArray())
        {
            QJsonArray stdoutArray = stdoutVal.toArray();
            for (auto o : stdoutArray)
            {
                if (o.isObject())
                {
                    QJsonObject outObj  = o.toObject();
                    QJsonValue  textVal = outObj["text"];
                    if (textVal.isString())
                    {
                        stdOut.append(textVal.toString() + "\n");
                    }
                }
            }
        }

        return true;
    }
}; // namespace PluginBase

#endif // PLUGINBASE_H
