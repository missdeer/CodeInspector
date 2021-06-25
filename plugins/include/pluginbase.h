#ifndef PLUGINBASE_H
#define PLUGINBASE_H

#include <QJsonArray>
#include <QJsonObject>

namespace PluginBase
{
    bool hasToolResult(const QJsonObject &docObj, const QString &name)
    {
        QJsonValue toolsVal = docObj["tools"];
        if (toolsVal.isArray())
        {
            auto toolsArray = toolsVal.toArray();
            auto it         = std::find_if(toolsArray.begin(), toolsArray.end(), [&name](QJsonValueRef v) {
                if (!v.isObject())
                    return false;

                auto o = v.toObject();
                if (o.contains("id") && o["id"].isString())
                    return o["id"].toString() == name;

                return false;
            });
            return toolsArray.end() != it;
        }

        return false;
    }
}; // namespace PluginBase

#endif // PLUGINBASE_H
