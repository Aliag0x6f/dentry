#pragma once
#include <QObject>
#include <QString>

namespace dentry::app::events {

class EventBus : public QObject {
    Q_OBJECT
public:
    static EventBus* instance();

signals:
    void focusWidget(const QString& widgetName);
};

} // namespace dentry::app::events

