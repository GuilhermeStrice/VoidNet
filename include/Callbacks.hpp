#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"
#include "Serializer.hpp"

void OnMessage(uint16 sender, uint16 tag, uint16 subject, void *data);

void OnDisconnect(uint16 id);

void OnConnect(uint16 id);

#endif