/*
-----------------------------------------------------------------------------
This source file is part of OGRE 
	(Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright (c)2000-2002 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under 
the terms of the GNU Lesser General Public License as published by the Free Software 
Foundation; either version 2 of the License, or (at your option) any later 
version.

This program is distributed in the hope that it will be useful, but WITHOUT 
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with 
this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
Place - Suite 330, Boston, MA 02111-1307, USA, or go to 
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#ifndef __ParticleEmitterCommands_H__
#define __ParticleEmitterCommands_H__

#include "Prerequisites.h"
#include "StringInterface.h"

namespace renderer  {


    namespace EmitterCommands {
        /// Command object for ParticleEmitter  - see ParamCommand 
        class _RendererExport CmdAngle : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdColour : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };

        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdColourRangeStart : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdColourRangeEnd : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };

        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdDirection : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };

        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdEmissionRate : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdVelocity : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdMinVelocity : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdMaxVelocity : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdTTL : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdMinTTL : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdMaxTTL : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdPosition : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdDuration : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdMinDuration : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdMaxDuration : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdRepeatDelay : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdMinRepeatDelay : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };
        /// Command object for particle emitter  - see ParamCommand 
        class _RendererExport CmdMaxRepeatDelay : public ParamCommand
        {
        public:
            String doGet(void* target);
            void doSet(void* target, const String& val);
        };

    }

}





#endif

