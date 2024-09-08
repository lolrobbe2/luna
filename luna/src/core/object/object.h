#pragma once
#ifndef _OBJECT_
#define _OBJECT_
#include <core/core.h>
#include <core/debug/debugMacros.h>
#include <core/debug/uuid.h>

#ifndef LN_OBJECT
#define LN_OBJECT(m_class) \
	void setHandleScene(entt::entity handle, luna::scene* scene) {entityHandle = handle;p_scene = scene;} \
    m_class() = default; \
    m_class(entt::entity handle, luna::scene* scene) \
        { \
        LN_ERR_FAIL_COND_MSG(handle == entt::null, "Invalid node quikID!"); \
		m_class::setHandleScene(handle,scene);\
    } \
    m_class(uint64_t id, luna::scene* scene) \
	{\
		LN_ERR_FAIL_COND_MSG(id == -1, "native id was invalid"); \
		auto idComponents = p_scene->m_Registry.view<idComponent, tagComponent>(); \
		auto it = std::find_if(idComponents.begin(), idComponents.end(), [&](entt::entity entity) { return getComponent<idComponent>().id == id; }); \
		if (it != idComponents.end()) \
			entityHandle = *it; \
	} \
	m_class(luna::scene* p_scene) {init(p_scene);} \
	virtual ~m_class() = default; \
	virtual void notification(notificationType type); \
    static const char* className() { return #m_class; } 
#endif // !LN_CLASS

#ifndef LN_CLASS
#define LN_CLASS(m_class, m_inherits) \
    static_assert(std::is_base_of<object,m_class>::value, "Class must derive from 'object'"); \
    m_class() = default; \
    m_class(entt::entity handle, luna::scene* scene) \
        : m_inherits(handle, scene), entityHandle(handle), scene(scene) { \
        LN_ERR_FAIL_COND_MSG(handle == entt::null, "Invalid node quikID!"); \
    } \
    m_class(uint64_t id, luna::scene* scene) { /* constructor body for id */ } \
	m_class(luna::scene* scene) {m_inherits::init(scene); init();} \
	virtual ~m_class() = default; \
	virtual void notification(notificationType type); \
	friend class m_inherits; \
    static const char* className() { return #m_class; } \
    static const char* baseClassName() { return #m_inherits; }
#endif // !LN_CLASS


namespace luna
{
	enum notificationType
	{
		NOTIFICATION_READY,
		TRANSFORM_UPDATED
	};

	struct idComponent
	{
		uuid id;
		std::string typeName;
		operator uint64_t() { return id; }
		std::function<void(notificationType)> notificationFunc = [](notificationType type) {};
		idComponent() = default;
		idComponent(const idComponent&) = default;
	};

	struct tagComponent
	{
		std::string tag = "Node";

		tagComponent() = default;
		tagComponent(const tagComponent&) = default;
		tagComponent(const std::string& tag)
			: tag(tag) {}
	};
	class LN_API scene;
	/**
	 * @brief object class.
	 * @warning DO NOT TOUCH UNLESS YOU KNOW WHAT YOURE DOING!!!
	 */
	class LN_API object
	{
	public:
		LN_OBJECT(object);
		virtual void init(scene* scene);
		virtual	void bindMethods();
		/*@brief emits a notification to a node or module*/
		virtual void notification(const notificationType type) {};
		/**
		* @brief emits a signal by name to all the nodes to wich the signal is connected
		* all the arguments need to be mono compatible! (for example std::string => MonoString*)
		*/
		template <typename ... ArgsT>
		void emitSignal(const char* functionName, ArgsT && ... inMonoArgs)
		{
			std::vector<void*> args = { static_cast<void*>(&inMonoArgs)... };
			if (args.size() == 0) emitSignalParams(functionName, nullptr);
			else emitSignalParams(functionName, args.data());
		}
		/**
		* @brief emits a signal by name to all the nodes to wich the signal is connected
		* all the arguments need to be mono compatible! (for example std::string => MonoString*)
		*/
		void emitSignalParams(const char* functionName, void** monoParams);

		/**
		* @brief connects a signal to a object, that ether being itself or another object.
		*
		* @param uint64_t objectID (quikID entt::entity)
		*/
		void connectSignal(uint64_t objectID, const std::string& functionName);
		std::vector<std::string> getSignalNames();
		template<typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			//LN_CORE_ASSERT(!hasComponent<T>(), "Node already has component!");
			T& component = scene->m_Registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
			//scene->onComponentAdded<T>(*this, component);
			return component;
		}
		template<typename T, typename... Args>
		T& addOrReplaceComponent(Args&&... args)
		{
			T& component = this->scene->m_Registry.emplace_or_replace<T>(entityHandle, std::forward<Args>(args)...);
			return component;
		}

		template<typename T>
		T& getComponent()
		{
			return scene->m_Registry.get<T>(entityHandle);
		}

		template<typename T>
		bool hasComponent()
		{
			return scene->m_Registry.all_of<T>(entityHandle);
		}
		template<typename T>
		void removeComponent()
		{
			this->scene->m_Registry.remove<T>(entityHandle);
		}
		uuid getUUID();
		operator entt::entity() { return entityHandle; }
	protected:
		friend class luna::scene;
		entt::entity entityHandle{ entt::null };
		scene* p_scene = nullptr;

	};
}
#endif // !_OBJECT_