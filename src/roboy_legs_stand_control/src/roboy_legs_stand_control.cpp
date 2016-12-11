/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <iostream>
#include <cstring>
#include "Leap.h"
#include <ros/ros.h>
#include <flexrayusbinterface/CommunicationData.h>
#include "flexrayusbinterface/FlexRayHardwareInterface.hpp"

ros::NodeHandlePtr nh;

using namespace Leap;

class SampleListener : public Listener {
public:

    virtual void onInit(const Controller &);

    virtual void onConnect(const Controller &);

    virtual void onDisconnect(const Controller &);

    virtual void onExit(const Controller &);

    virtual void onFrame(const Controller &);

    virtual void onFocusGained(const Controller &);

    virtual void onFocusLost(const Controller &);

    virtual void onDeviceChange(const Controller &);

    virtual void onServiceConnect(const Controller &);

    virtual void onServiceDisconnect(const Controller &);

private:
    FlexRayHardwareInterface flexRayHardwareInterface;

};

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

void SampleListener::onInit(const Controller &controller) {
    std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller &controller) {
    std::cout << "Connected" << std::endl;
    controller.enableGesture(Gesture::TYPE_CIRCLE);
    controller.enableGesture(Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
    controller.enableGesture(Gesture::TYPE_SWIPE);
    if (!ros::isInitialized()) {
        int argc = 0;
        char **argv = NULL;
        ros::init(argc, argv, "roboy_legs_stand_control", ros::init_options::NoSigintHandler);
    }
    nh = ros::NodeHandlePtr(new ros::NodeHandle);
}

void SampleListener::onDisconnect(const Controller &controller) {
    // Note: not dispatched when running in a debugger.
    std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller &controller) {
    std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller &controller) {
    // Get the most recent frame and report some basic information
    const Frame frame = controller.frame();
    HandList hands = frame.hands();
    Hand hand = *hands.begin();
    if(hand.palmPosition().y>0) {
        ROS_INFO_STREAM_THROTTLE(1,"HandPosition: " << hand.palmPosition().y);
        flexRayHardwareInterface.commandframe0[0].sp[0] = hand.palmPosition().y/300*400;
        flexRayHardwareInterface.exchangeData();
        flexRayHardwareInterface.updateCommandFrame();
    }else{
        ROS_INFO_STREAM_THROTTLE(5,"no hand detected");
    }
}

void SampleListener::onFocusGained(const Controller &controller) {
    std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller &controller) {
    std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller &controller) {
    std::cout << "Device Changed" << std::endl;
    const DeviceList devices = controller.devices();

    for (int i = 0; i < devices.count(); ++i) {
        std::cout << "id: " << devices[i].toString() << std::endl;
        std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
    }
}

void SampleListener::onServiceConnect(const Controller &controller) {
    std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller &controller) {
    std::cout << "Service Disconnected" << std::endl;
}

int main(int argc, char **argv) {
    // Create a sample listener and controller
    SampleListener listener;
    Controller controller;

    // Have the sample listener receive events from the controller
    controller.addListener(listener);

    if (argc > 1 && strcmp(argv[1], "--bg") == 0)
        controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

    // Keep this process running until Enter is pressed
    std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get();

    // Remove the sample listener when done
    controller.removeListener(listener);

    return 0;
}

