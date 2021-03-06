#include "core/neuron/god/main.hpp"
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
    NeuronSptr  neuron;
    UiViewer    *ui_viewer = getUiViewer();

    // init nerves audio
    m_nerves_audio  = new HumanNervesAudioInput(this);
    m_nerves_audio->show();

    // init model
    m_model = new HumanModelAudioFlame(this);
    neuron  = m_model->init();

    // center
    ui_viewer->setCenterNeuron(neuron);

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
    NeuronSptr  neuron;
    UiViewer    *ui_viewer = getUiViewer();

    // init nerves audio
    // m_nerves_audio  = new HumanNervesAudioInput(this);
    // m_nerves_audio->show();

    // init nerves video
    // m_nerves_video = new HumanNervesVideoWebCam(this);

    // init model
    m_model = new HumanModelBrain(this);
    neuron = m_model->init();

    // center
    ui_viewer->setCenterNeuron(neuron);
}

void HumanViewer::initExperimentPhysics()
{
    NeuronSptr  neuron;
    UiViewer    *ui_viewer = getUiViewer();

    // init nerves audio
    // m_nerves_audio  = new HumanNervesAudioInput(this);
    // m_nerves_audio->show();

    // init nerves video
    // m_nerves_video = new HumanNervesVideoWebCam(this);

    // init model
    m_model = new HumanModelPhysics(this);
    neuron = m_model->init();

    // center
    ui_viewer->setCenterNeuron(neuron);
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

Network * HumanViewer::getNetwork()
{
    return m_network;
}

DbThread * HumanViewer::getDbThread()
{
    DbThread *db = NULL;

    db = m_network->getDbThread();

    return db;
}

/*
void HumanViewer::loadModel()
{
    DbThread    *db      = m_network->getDbThread();
    NeuronSptr  neuron   = a_neuron;

    vector<NeuronSptr>              neighs;
    vector<NeuronSptr>::iterator    neighs_it;

    float               x, y, z, ang_x, ang_y;
    NeuronsIt           it;
    string              id;
    UiObjectNeuronSptr  object_neuron;
    uint32_t            index = 0;

    if (!neuron){
        neuron = db->getNeuronById(PropNeuronIdSptr(
            new PropNeuronId(
              "sphere"
            )
        ));
        if (!neuron){
            neuron = createModel();
        }
    }

    id = neuron->getId()->toString();
    it = m_neurons.find(id);
    if (m_neurons.end() != it){
        // already loaded
        goto out;
    }

    // create ui object
    index = (neuron->morik_row << 16)
        + neuron->morik_col;
    object_neuron = UiObjectNeuronSptr(new UiObjectNeuron(
        this));
    object_neuron->m_neuron  = neuron;
    m_objects.push_back(object_neuron);

    m_objects_neurons[index] = object_neuron;

    if (!a_neuron){
        // store root neuron to different variable too
        m_object_neuron = object_neuron;
    }

    // store us in hash by name (for uniq checking)
    m_neurons[id] = neuron;

    ang_x = a_info.m_ang_x;
    ang_y = a_info.m_ang_y;

    x = sin(ang_y) * cos(ang_x)   * a_info.m_radius;
    y = sin(0 - ang_x)            * a_info.m_radius;
    z = cos(ang_y) * cos(ang_x)   * a_info.m_radius;

    object_neuron->m_frame.setPosition(
        Vec(x, y, z)
    );

    // prepare for next level
    a_info.m_radius     += 1.0f;
    a_info.m_level      += 1;
    a_info.m_x          = x;
    a_info.m_y          = y;
    a_info.m_z          = z;
    a_info.m_neuron     = object_neuron;

    // get neighs
    neuron->getNeighs<Neuron>(neighs);
    sort(neighs.begin(), neighs.end());
    for (neighs_it = neighs.begin();
        neighs_it != neighs.end();
        neighs_it++)
    {
        NeuronSptr  cur_neuron = *neighs_it;
        string      cur_id;
        NeuronsIt   cur_it;
        LoadInfo    tmp;

        cur_id = cur_neuron->getId()->toString();
        cur_it = m_neurons.find(cur_id);

        if (m_neurons.end() != it){
            // already loaded
            continue;
        }

        ang_x = -1.5f + 3.0f*cur_neuron->morik_row/(float(YUE_HEIGHT));
        ang_y = -2.0f + 2.0f*cur_neuron->morik_col/(float(YUE_WIDTH));
        //ang_y = -2.0f + 6.3f*cur_neuron->morik_col/(float(YUE_WIDTH));

        // prepare for next level
        a_info.m_ang_x = ang_x;
        a_info.m_ang_y = ang_y;

        load_sphere(cur_neuron, a_info);
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

void HumanViewer::getNeighNeurons(
    NeuronSptr          a_neuron,
    vector<NeuronSptr>  &a_out,
    double              a_radius)
{
    UiViewer *ui_viewer = getUiViewer();

    ui_viewer->getNeighNeurons(
        a_neuron,
        a_out,
        a_radius
    );
}

