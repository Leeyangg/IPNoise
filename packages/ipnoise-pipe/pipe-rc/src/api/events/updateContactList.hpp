#ifndef API_EVENT_UPDATE_CONTACT_LIST
#define API_EVENT_UPDATE_CONTACT_LIST

#include "apiEvent.hpp"

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

class ApiEventUpdateContactList
    :   public ApiEvent
{
    Q_OBJECT

    public:
        ApiEventUpdateContactList(Api *a_api);
        virtual ~ApiEventUpdateContactList();

        virtual void process();
};


#endif

