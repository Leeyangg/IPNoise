#include "class.hpp"
DEFINE_CLASS(HumanViewer);

#ifndef HUMAN_VIEWER_HPP
#define HUMAN_VIEWER_HPP

#include <stdint.h>

#include <QObject>
#include <QStringList>

#include <string>

DEFINE_CLASS(Neuron);
DEFINE_CLASS(DbThread);
DEFINE_CLASS(Network);

//#include "network.hpp"
//#include "core/neuron/neuron/main.hpp"
#include "human/nerves.hpp"

#include "human/nerves/audio.hpp"
#include "human/nerves/video.hpp"

#include "ui/viewer.hpp"
#include "human/model.hpp"

#define UI_VIWER_NEURON_SPHERE_SIZE 0.01f

using namespace std;

class HumanViewer
    :   public QObject
{
    Q_OBJECT

    public:
        HumanViewer(
            Network       *a_network,
            QObject       *a_parent = 0
        );
        virtual ~HumanViewer();

        Network             *   getNetwork();
        DbThread            *   getDbThread();
        UiViewer            *   getUiViewer();
        HumanNervesVideo    *   getHumanNervesVideo();
        HumanNervesAudio    *   getHumanNervesAudio();

        void    initExperimentSphere();
        void    initExperimentAudioFlame();
        void    initExperimentBrain();
        void    initExperimentPhysics();

        void getNeighNeurons(
            NeuronSptr          a_neuron,
            vector<NeuronSptr>  &a_out,
            double              a_radius = UI_VIWER_NEURON_SPHERE_SIZE
        );

    private:
        Network                 *m_network;

        HumanNervesAudio        *m_nerves_audio;
        HumanNervesVideo        *m_nerves_video;

        HumanModel              *m_model;
};

#endif

