/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include <RGB2HSV.h>

USING_NS_CC;

const int HelloWorld::BackgroundTag = 1;
const uint8_t HelloWorld::ColorAngleRotation = 30;
const uint8_t HelloWorld::ScrollAngleCoeficient = 5;
const uint8_t HelloWorld::MoveAngleCoeficient = 2;
const uint8_t HelloWorld::AccelerateAngleCoeficient = 10;

Vec3 HelloWorld::_defaultAcceleration = Vec3(0, 9.81, 0);


Vec2 HelloWorld::_oldPoint = Vec2();

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    _oldPoint = cocos2d::Vec2();

    auto _mouseListener = EventListenerMouse::create();
    
    _mouseListener->onMouseDown = HelloWorld::onMouseDown;
    _mouseListener->onMouseMove = HelloWorld::onMouseMove;
    _mouseListener->onMouseScroll = HelloWorld::onMouseScroll;
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    auto keyboardlistener = EventListenerKeyboard::create();

    keyboardlistener->onKeyPressed = HelloWorld::onKeyPressed;

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener, this);
   
    auto touchListener = EventListenerTouchOneByOne::create();

    touchListener->onTouchBegan = HelloWorld::onTouchBegan;
    touchListener->onTouchMoved = HelloWorld::onTouchMoved;
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    auto accelerationListener = EventListenerAcceleration::create(HelloWorld::onAcceleration);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(accelerationListener, this);

    Device::setAccelerometerEnabled(true);

    LayerColor* _bgColor = LayerColor::create(Color4B(255, 255, 0, 255));
    this->addChild(_bgColor, -10, 1);

    return true;
}

void HelloWorld::rotateColor(cocos2d::Node* node, uint8_t angle)
{
    LayerColor* bColor = dynamic_cast<LayerColor*>(node->getChildByTag(BackgroundTag));

    if (!bColor)
    {
        return;
    }

    Color3B oldColor = RGB2HSV::rgb2hsv(bColor->getColor());

    oldColor.r += angle;

    Color3B newColor = RGB2HSV::hsv2rgb(oldColor);

    bColor->setColor(newColor);
}

void HelloWorld::setColor(cocos2d::Node* node, uint8_t angle)
{
    LayerColor* bColor = dynamic_cast<LayerColor*>(node->getChildByTag(BackgroundTag));

    if (!bColor)
    {
        return;
    }

    Color3B newColor = RGB2HSV::hsv2rgb(Color3B(angle, 255, 255));

    bColor->setColor(newColor);
}

void HelloWorld::onMouseDown(EventMouse* event)
{
    Node* node = event->getCurrentTarget();
    EventMouse::MouseButton button = event->getMouseButton();

    if (button == EventMouse::MouseButton::BUTTON_LEFT)
    {
        rotateColor(node, ColorAngleRotation);
        return;
    }

    rotateColor(node, -ColorAngleRotation);
}

void HelloWorld::onMouseScroll(EventMouse* event)
{
    Node* node = event->getCurrentTarget();

    rotateColor(node, event->getScrollY() * ScrollAngleCoeficient);
}

void HelloWorld::onMove(Vec2 location, Node* node)
{
    if (_oldPoint.lengthSquared() < 0.01)
    {
        _oldPoint = location;
        return;
    }

    Vec2 delta = location - _oldPoint;

    uint8_t angle = (delta.x + delta.y) > 0 ? MoveAngleCoeficient : -MoveAngleCoeficient;

    rotateColor(node, angle);

    _oldPoint = location;
}

void HelloWorld::onMouseMove(EventMouse* event)
{
    Vec2 location = event->getLocationInView();

    Node* node = event->getCurrentTarget();

    onMove(location, node);
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    Node* node = event->getCurrentTarget();

    setColor(node, (uint8_t)keyCode);
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    Node* node = event->getCurrentTarget();

    rotateColor(node, ColorAngleRotation);
    return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
    Vec2 location = touch->getLocationInView();
    Node* node = event->getCurrentTarget();

    onMove(location, node);
}

void HelloWorld::onAcceleration(Acceleration* acc, Event* event)
{
    Vec3 location = Vec3(acc->x, acc->y, acc->y);
    Node* node = event->getCurrentTarget();
    Vec3 delta = location - _defaultAcceleration;

    if (delta.lengthSquared() < 0.1)
    {
        return;
    }

    rotateColor(node, AccelerateAngleCoeficient);
    _defaultAcceleration = location;
}