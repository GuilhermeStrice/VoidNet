#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"
#include "Serializer.hpp"

void OnMessage(byte sender, byte tag, uint16 subject, byte *data);

void OnDisconnect(byte id);

void OnConnect(byte id);

#endif