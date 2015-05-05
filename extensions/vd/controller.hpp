#pragma once

#include "shared.hpp"
#include "singleton.hpp"
#include "penetration.hpp"
#include "arguments.hpp"
#include "dispatch.hpp"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace ace {
    namespace vehicledamage {
        class controller 
            : public singleton<controller>,
            public threaded_dispatcher {
        public:
            controller();
            ~controller();

            bool fetch_result(const arguments & args, std::string & result);

            bool register_vehicle(const arguments & args, std::string & result);
            bool delete_vehicle(const arguments & args, std::string & result);

            bool handle_hit(const arguments & args, std::string & result);

#ifdef _DEBUG
            bool _test_raycast(const arguments & args, std::string & result);
            bool _test_selection(const arguments & args, std::string & result);
#endif

            // Bullet physx world
            std::shared_ptr<btDefaultCollisionConfiguration>    bt_collisionConfiguration;
            std::shared_ptr<btCollisionDispatcher>              bt_collisionDispatcher;
            std::shared_ptr<btCollisionWorld>                   bt_world;

            std::map<uint32_t, ace::vehicledamage::vehicle_p>     vehicles;
        };

    }
}
