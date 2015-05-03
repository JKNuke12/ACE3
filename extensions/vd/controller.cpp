#include "controller.hpp"
#include "model_collection.hpp"
#include "simulation/object.hpp"

#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"

namespace ace {
    namespace vehicledamage {
        controller::controller() : dispatcher() { 
            btBroadphaseInterface* broadphase = new btDbvtBroadphase();

            // Set up the collision configuration and dispatcher
            bt_collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
            bt_collisionDispatcher = std::make_shared<btCollisionDispatcher>(bt_collisionConfiguration.get());

            // The world.
            bt_world = std::make_shared<btCollisionWorld>(bt_collisionDispatcher.get(), broadphase, bt_collisionConfiguration.get());

            // Register functions
            add("register_vehicle", std::bind(&ace::vehicledamage::controller::handle_hit, this, std::placeholders::_1, std::placeholders::_2));
            add("hit", std::bind(&ace::vehicledamage::controller::handle_hit, this, std::placeholders::_1, std::placeholders::_2));
            
#ifdef _DEBUG
            add("test_raycast", std::bind(&ace::vehicledamage::controller::_test_raycast, this, std::placeholders::_1, std::placeholders::_2));
            add("test_selection", std::bind(&ace::vehicledamage::controller::_test_selection, this, std::placeholders::_1, std::placeholders::_2));
#endif
        }
        controller::~controller() { }

        bool controller::register_vehicle(const arguments &_args, const std::string & result) {
            return false;
        }
        /*
        This is sent in via SQF by a callExtension call, with the variables:

        // A vector is formatted in x;y;z

        object_id,
        classname, selection, projectile_classname, projectile_density,
        projectile_length, projectile_diameter, projectile_velocity, projectile_position, projectile_direction,
        orthogonal_surface, impact_location, impact_velocity

        class gamehit {
        public:
        uint32_t            _id;
        uint32_t            _object_id;

        std::string         _classname;
        std::string         _selection;

        projectile          _projectile;

        float               _orthogonal_surface;
        ace::vector3<float> _impact_location;
        ace::vector3<float> _impact_velocity;
        };
        */
        bool controller::handle_hit(const arguments &_args, const std::string & result) {
            if (_args.size() < 13) return false;

            gamehit_p _hit = gamehit::create(_args);

            ace::simulation::object * _object = new ace::simulation::object(model_collection::get().models[0].model);
            std::shared_ptr<ace::simulation::object> _object_ptr(_object);
            ace::vehicledamage::vehicle _vehicle(12345, _object_ptr);

            btVector3 velocity(_hit->impactvelocity.x(), _hit->impactvelocity.y(), _hit->impactvelocity.z());
            btVector3 from(_hit->impactposition.x(), _hit->impactposition.y(), _hit->impactposition.z());
            btVector3 to = from + (velocity * 100);

            btCollisionWorld::AllHitsRayResultCallback allResults(from, to);
            bt_world->rayTest(from, to, allResults);

            LOG(INFO) << "Raycast test conducted";
            std::stringstream ss;
            for (int x = 0; x < allResults.m_hitPointWorld.size(); x++) {
                LOG(INFO) << "pointWorld[" << x << "] = " << allResults.m_hitPointWorld[x].x() << ", " << allResults.m_hitPointWorld[x].y() << ", " << allResults.m_hitPointWorld[x].z();
                ss << " [";
                ss << allResults.m_hitPointWorld[x].x() << ", " << allResults.m_hitPointWorld[x].y() << ", " << allResults.m_hitPointWorld[x].z();
                ss << " ], ";
            }
            for (int x = 0; x < allResults.m_collisionObjects.size(); x++) {
                LOG(INFO) << "collisionObjects[" << x << "] = " << allResults.m_collisionObjects[x]->getUserIndex();
            }
            if (allResults.m_collisionObjects.size() > 1) {
                LOG(INFO) << "Thickness at direction: " << allResults.m_hitPointWorld[0].distance(allResults.m_hitPointWorld[1]);
            }
            if(ss.str().length() > 1)
                ss << "]";

            LOG(INFO) << "Result: " << ss.str();

            return false;
        }

#ifdef _DEBUG
        bool controller::_test_raycast(const arguments &_args, const std::string & result) {
            ace::simulation::object * _object = new ace::simulation::object(model_collection::get().models[0].model);
            std::shared_ptr<ace::simulation::object> _object_ptr(_object);
            ace::vehicledamage::vehicle _vehicle(12345, _object_ptr);

            btVector3 from(1000.0f, 1000.0f, 1000.0f), to(-1000.0f, -1000.0f, -1000.0f);
           
            btCollisionWorld::AllHitsRayResultCallback allResults(from, to);
            bt_world->rayTest(from, to, allResults);

            LOG(INFO) << "Raycast test conducted";
            std::stringstream ss;
            for (int x = 0; x < allResults.m_hitPointWorld.size(); x++) {
                LOG(INFO) << "pointWorld[" << x << "] = " << allResults.m_hitPointWorld[x].x() << ", " << allResults.m_hitPointWorld[x].y() << ", " << allResults.m_hitPointWorld[x].z();
                ss << " [";
                ss << allResults.m_hitPointWorld[x].x() << ", " << allResults.m_hitPointWorld[x].y() << ", " << allResults.m_hitPointWorld[x].z();
                ss << " ], ";
            }
            for (int x = 0; x < allResults.m_collisionObjects.size(); x++) {
                LOG(INFO) << "collisionObjects[" << x << "] = " << allResults.m_collisionObjects[x]->getUserIndex();
            }
            if (allResults.m_collisionObjects.size() > 1) {
                LOG(INFO) << "Thickness at direction: " << allResults.m_hitPointWorld[0].distance(allResults.m_hitPointWorld[1]);
                
            }
            
            ss << "]";
            LOG(INFO) << "Result: " << ss.str();

            return true;
        }
        bool controller::_test_selection(const arguments &_args, const std::string & result) {
            ace::simulation::object * _object = new ace::simulation::object(model_collection::get().models[0].model);
            std::shared_ptr<ace::simulation::object> _object_ptr(_object);
            ace::vehicledamage::vehicle _vehicle(12345, _object_ptr);
            
            std::vector<ace::vector3<float>> _vertices = _vehicle.selection_by_name_vertices(-1, _args[0]);

            std::stringstream ss;
            ss << "[";
            for (auto & v : _vertices) {
                ss << " [";
                ss << v.x() << ", " << v.y() << ", " << v.z();
                ss << " ], ";
            }
            ss << "]";
            LOG(INFO) << "Result: " << ss.str();
            
            return true;
        }
#endif
    }
}
