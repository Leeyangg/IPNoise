#include "core/object/god/main.hpp"
#include "network.hpp"

#include "human/viewer.hpp"

#include "human/model.hpp"
#include "human/model/sphere.hpp"
#include "human/model/audioFlame.hpp"
#include "human/model/audioTest.hpp"
#include "human/model/brain.hpp"
#include "human/model/physics.hpp"

#include "human/nerves/audio/input.hpp"
#include "human/nerves/video/webcam.hpp"
#include "human/nerves/video/test.hpp"

HumanViewer::HumanViewer(
    Network         *a_network,
    QObject         *a_parent)
    :   QObject(a_parent)
{
    m_network       = a_network;
    m_nerves_audio  = NULL;
    m_nerves_video  = NULL;
    m_model         = NULL;

    // init experiment
    // initExperimentSphere();
    // initExperimentAudioFlame();
    // initExperimentBrain();
    initExperimentPhysics();
}

HumanViewer::~HumanViewer()
{
}

UiViewer * HumanViewer::getUiViewer()
{
    UiViewer *ui_viewer = NULL;
    ui_viewer = m_network->getUiViewer();
    return ui_viewer;
}

void HumanViewer::initExperimentAudioFlame()
{
    ObjectSptr  object;
    UiViewer    *ui_viewer = getUiViewer();

    // init nerves audio
    m_nerves_audio  = new HumanNervesAudioInput(this);
    m_nerves_audio->show();

    // init model
    m_model = new HumanModelAudioFlame(this);
    object  = m_model->init();

    // center
    ui_viewer->setCenterObject(object);

    if (0){
        HumanModel *model = NULL;
        model = new HumanModelAudioTest(this);
        model->init();
    }
}

void HumanViewer::initExperimentSphere()
{
    // init nerves audio
    m_nerves_audio  = new HumanNervesAudioInput(this);
    m_nerves_audio->show();

    // init nerves video
    m_nerves_video = new HumanNervesVideoWebCam(this);

    // init model
    m_model = new HumanModelSphere(this);
    m_model->init();
}

void HumanViewer::initExperimentBrain()
{
    ObjectSptr  object;
    UiViewer    *ui_viewer = getUiViewer();

    // init nerves audio
    // m_nerves_audio  = new HumanNervesAudioInput(this);
    // m_nerves_audio->show();

    // init nerves video
    // m_nerves_video = new HumanNervesVideoWebCam(this);

    // init model
    m_model = new HumanModelBrain(this);
    object = m_model->init();

    // center
    ui_viewer->setCenterObject(object);
}

void HumanViewer::initExperimentPhysics()
{
    ObjectSptr  object;
    UiViewer    *ui_viewer = getUiViewer();

    // init nerves audio
    // m_nerves_audio  = new HumanNervesAudioInput(this);
    // m_nerves_audio->show();

    // init nerves video
    // m_nerves_video = new HumanNervesVideoWebCam(this);

    // init model
    m_model = new HumanModelPhysics(this);
    object = m_model->init();

    // center
    ui_viewer->setCenterObject(object);
}

HumanNervesVideo * HumanViewer::getHumanNervesVideo()
{
    return m_nerves_video;
}

HumanNervesAudio * HumanViewer::getHumanNervesAudio()
{
    return m_nerves_audio;
}

/*
int32_t HumanViewer::do_init()
{
    int32_t err = 0;

//    createModel();

    return err;
}
*/

/*
void HumanViewer::loadModel()
{
    ObjectSptr  object   = a_object;

    vector<ObjectSptr>              neighs;
    vector<ObjectSptr>::iterator    neighs_it;

    float               x, y, z, ang_x, ang_y;
    ObjectsIt           it;
    string              id;
    UiObjectObjectSptr  object_object;
    uint32_t            index = 0;

    if (!object){
        object = ThreadDb::get()->getObjectById(ObjectIdSptr(
            new ObjectId(
              "sphere"
            )
        ));
        if (!object){
            object = createModel();
        }
    }

    id = object->getId()->toString();
    it = m_objects.find(id);
    if (m_objects.end() != it){
        // already loaded
        goto out;
    }

    // create ui object
    index = (object->morik_row << 16)
        + object->morik_col;
    object_object = UiObjectObjectSptr(new UiObjectObject(
        this));
    object_object->m_object  = object;
    m_objects.push_back(object_object);

    m_objects_objects[index] = object_object;

    if (!a_object){
        // store root object to different variable too
        m_object_object = object_object;
    }

    // store us in hash by name (for uniq checking)
    m_objects[id] = object;

    ang_x = a_info.m_ang_x;
    ang_y = a_info.m_ang_y;

    x = sin(ang_y) * cos(ang_x)   * a_info.m_radius;
    y = sin(0 - ang_x)            * a_info.m_radius;
    z = cos(ang_y) * cos(ang_x)   * a_info.m_radius;

    object_object->m_frame.setPosition(
        Vec(x, y, z)
    );

    // prepare for next level
    a_info.m_radius     += 1.0f;
    a_info.m_level      += 1;
    a_info.m_x          = x;
    a_info.m_y          = y;
    a_info.m_z          = z;
    a_info.m_object     = object_object;

    // get neighs
    object->getNeighs<Object>(neighs);
    sort(neighs.begin(), neighs.end());
    for (neighs_it = neighs.begin();
        neighs_it != neighs.end();
        neighs_it++)
    {
        ObjectSptr  cur_object = *neighs_it;
        string      cur_id;
        ObjectsIt   cur_it;
        LoadInfo    tmp;

        cur_id = cur_object->getId()->toString();
        cur_it = m_objects.find(cur_id);

        if (m_objects.end() != it){
            // already loaded
            continue;
        }

        ang_x = -1.5f + 3.0f*cur_object->morik_row/(float(YUE_HEIGHT));
        ang_y = -2.0f + 2.0f*cur_object->morik_col/(float(YUE_WIDTH));
        //ang_y = -2.0f + 6.3f*cur_object->morik_col/(float(YUE_WIDTH));

        // prepare for next level
        a_info.m_ang_x = ang_x;
        a_info.m_ang_y = ang_y;

        load_sphere(cur_object, a_info);
    }

out:
    return;

}

void HumanViewer::do_start()
{
    while (Viewer::THREAD_STATE_RUNNING == getState()){
        PWARN("'%s' thread tick\n",
            getName().c_str()
        );

//        NervesGroupsIt  it;
//        for (it = m_nerves_groups.begin();
//            it != m_nerves_groups.end();
//            it++)
//        {
//            NervesGroupSptr group = *it;
//            group->process();
//        }

        sleep(1);
    }
}

void HumanViewer::do_stop()
{
    PWARN("'%s' thread do_stop\n",
        getName().c_str()
    );
}
*/

void HumanViewer::getNeighObjects(
    ObjectSptr          a_object,
    vector<ObjectSptr>  &a_out,
    double              a_radius)
{
    UiViewer *ui_viewer = getUiViewer();

    ui_viewer->getNeighObjects(
        a_object,
        a_out,
        a_radius
    );
}

