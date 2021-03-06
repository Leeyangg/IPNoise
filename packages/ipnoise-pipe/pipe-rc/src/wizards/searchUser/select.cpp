#include "links/udp.hpp"
#include "links/tcp.hpp"
#include "select.hpp"

WizardSearchUserPageSelect::WizardSearchUserPageSelect(
    Api                 *a_api,
    ContactListPath     &a_contact_path,
    QWidget             *a_parent)
    :   QWizardPage(a_parent)
{
    m_api           = a_api;
    m_contact_path  = a_contact_path;

    REGISTER_LINK(Udp);
    REGISTER_LINK(Tcp);

    setupUi(this);
}

WizardSearchUserPageSelect::~WizardSearchUserPageSelect()
{
}

void WizardSearchUserPageSelect::createLinksCombo()
{
    LinksIt links_it;

    // type
    m_combo_links = new QComboBox;
    m_combo_links->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Fixed
    );
    m_combo_links->addItem("Select link type",   "");

    for (links_it = m_links.begin();
        links_it != m_links.end();
        links_it++)
    {
        LinkWidget *contact = links_it.value();
        m_combo_links->addItem(
            contact->getIcon(),
            contact->getDisplayName(),
            contact->getName()
        );
    }

    m_vbox->addWidget(m_combo_links);
}

void WizardSearchUserPageSelect::setupUi(QWidget *a_parent)
{
    QString pixmap_str;

    pixmap_str = absolutePathFromUnixPath(
        "resources/default/images/wizard_searchUser_select.png"
    );

    setTitle(tr("Search via link.."));
    setSubTitle(tr("Please specify link type and press next"));
    setPixmap(
        QWizard::LogoPixmap,
        QPixmap(pixmap_str)
    );

    m_vbox = new QVBoxLayout;

    createLinksCombo();

    m_vbox->addStretch(1);

    a_parent->setLayout(m_vbox);
}

LinkWidget * WizardSearchUserPageSelect::getLink() const
{
    LinksConstIt    links_it;
    LinkWidget      *link_widget = NULL;
    QString         link_type;

    link_type = m_combo_links->itemData(
        m_combo_links->currentIndex()
    ).toString();

    links_it = m_links.find(link_type);
    if (m_links.end() != links_it){
        link_widget = links_it.value();
    }

    return link_widget;
}

int WizardSearchUserPageSelect::nextId() const
{
    int         ret     = 0;
    LinkWidget *link    = getLink();

    if (!link){
        goto out;
    }

    if ("tcp_v4" == link->getName()){
        ret = Wizard::Page_WizardSearchUserPageFillTcpV4;
    } else if ("udp_v4" == link->getName()){
        ret = Wizard::Page_WizardSearchUserPageFillUdpV4;
    }

out:
    return ret;
}

