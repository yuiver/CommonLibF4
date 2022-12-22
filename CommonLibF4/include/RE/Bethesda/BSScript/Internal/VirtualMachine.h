#pragma once

#include "RE/Bethesda/BSFixedString.h"
#include "RE/Bethesda/BSLock.h"
#include "RE/Bethesda/BSScript/IClientVM.h"
#include "RE/Bethesda/BSScript/IVMDebugInterface.h"
#include "RE/Bethesda/BSScript/IVMObjectBindInterface.h"
#include "RE/Bethesda/BSScript/IVMSaveLoadInterface.h"
#include "RE/Bethesda/BSScript/IVirtualMachine.h"
#include "RE/Bethesda/BSScript/Internal/AttachedScript.h"
#include "RE/Bethesda/BSScript/Internal/FunctionMessage.h"
#include "RE/Bethesda/BSScript/Internal/ReadableStringTable.h"
#include "RE/Bethesda/BSScript/Internal/SuspendedStack.h"
#include "RE/Bethesda/BSScript/LinkerProcessor.h"
#include "RE/Bethesda/BSTArray.h"
#include "RE/Bethesda/BSTFreeList.h"
#include "RE/Bethesda/BSTHashMap.h"
#include "RE/Bethesda/BSTMessageQueue.h"
#include "RE/Bethesda/BSTSmartPointer.h"


namespace RE
{
	namespace BSScript
	{
		class Array;
		class ErrorLogger;
		class IFreezeQuery;
		class IProfilePolicy;
		class ISavePatcherInterface;
		class IStackCallbackFunctor;
		class IStackCallbackSaveInterface;
		class ObjectBindPolicy;
		class Stack;
		struct IMemoryPagePolicy;
		struct IObjectHandlePolicy;
		struct StatsEvent;

		namespace Internal
		{
			class CodeTasklet;
			class UnkKey;
			class UnkValue;
			class VirtualMachine :
				public IVirtualMachine,  // 0000
				//IVM -> public BSIntrusiveRefCounted  // 0008
				public IVMObjectBindInterface,     // 0010
				public IVMSaveLoadInterface,       // 0018
				public IVMDebugInterface,          // 0020
				public BSTEventSource<StatsEvent>  // 0028
			{
			public:
				static constexpr auto RTTI{ RTTI::BSScript__Internal__VirtualMachine };
				static constexpr auto VTABLE{ VTABLE::BSScript__Internal__VirtualMachine };

				struct QueuedUnbindRefs
				{
				public:
					// members
					BSTSmartPointer<Object> obj;  // 00
					std::uint32_t refCount;       // 08
					std::uint32_t pad0C;          // 0C
				};
				static_assert(sizeof(QueuedUnbindRefs) == 0x10);

				enum class FreezeState
				{
					kNotFrozen = 0,
					kFreezing,
					kFrozen
				};

				~VirtualMachine() override;  // 00

				using RE::BSScript::IVirtualMachine::SendEvent;
				//using RE::BSTEventSource<RE::BSScript::StatsEvent>::SendEvent;

			public:
				// override (IVirtualMachine)
				virtual void SetLoader(ILoader* a_newLoader) override;                                                                                                                                                                                                                                                           // 01
				virtual void SetLinkedCallback(ITypeLinkedCallback* a_typeLinkedCallback) override;                                                                                                                                                                                                                              // 02
				virtual void Update(float a_updateBudget) override;                                                                                                                                                                                                                                                              // 03
				virtual void UpdateTasklets(float a_updateBudget) override;                                                                                                                                                                                                                                                      // 04
				virtual void SetOverstressed(bool a_overstressed) override;                                                                                                                                                                                                                                                      // 05
				virtual bool IsCompletelyFrozen() const override;                                                                                                                                                                                                                                                                // 06
				virtual bool RegisterObjectType(std::uint32_t a_typeID, const char* a_objectTypeName) override;                                                                                                                                                                                                                  // 07
				virtual bool GetScriptObjectType(std::uint32_t a_typeID, BSTSmartPointer<ObjectTypeInfo>& a_objType) override;                                                                                                                                                                                                   // 09
				virtual bool GetScriptObjectType(const BSFixedString& a_name, BSTSmartPointer<ObjectTypeInfo>& a_objType) override;                                                                                                                                                                                              // 08
				virtual bool GetScriptObjectTypeNoLoad(std::uint32_t a_typeID, BSTSmartPointer<ObjectTypeInfo>& a_objType) const override;                                                                                                                                                                                       // 0B
				virtual bool GetScriptObjectTypeNoLoad(const BSFixedString& a_objectTypeName, BSTSmartPointer<ObjectTypeInfo>& a_objType) const override;                                                                                                                                                                        // 0A
				virtual bool GetTypeIDForScriptObject(const BSFixedString& a_objectTypeName, std::uint32_t& a_typeID) const override;                                                                                                                                                                                            // 0C
				virtual void GetScriptObjectsWithATypeID(BSScrapArray<BSFixedString>& a_objectTypeList) const override;                                                                                                                                                                                                          // 0D
				virtual bool GetParentNativeType(const BSFixedString& a_childTypeName, BSTSmartPointer<ObjectTypeInfo>& a_parentType) override;                                                                                                                                                                                  // 0E
				virtual bool TypeIsValid(const BSFixedString& a_objectTypeName) override;                                                                                                                                                                                                                                        // 0F
				virtual bool ReloadType(const char* a_objectTypeName) override;                                                                                                                                                                                                                                                  // 10
				virtual void TasksToJobs(JobList& a_jobList) override;                                                                                                                                                                                                                                                           // 11
				virtual void CalculateFullReloadList(const BSTSet<BSFixedString>& a_scriptSet, BSTObjectArena<BSFixedString>& a_scriptList) const override;                                                                                                                                                                      // 12
				virtual bool GetScriptStructType(const BSFixedString& a_structTypeName, BSTSmartPointer<StructTypeInfo>& a_structType) override;                                                                                                                                                                                 // 13
				virtual bool GetScriptStructTypeNoLoad(const BSFixedString& a_structTypeName, BSTSmartPointer<StructTypeInfo>& a_structType) const override;                                                                                                                                                                     // 14
				virtual bool GetChildStructTypes(const BSFixedString& a_parentObjectName, BSTObjectArena<BSFixedString>& a_structTypes) const override;                                                                                                                                                                          // 15
				virtual bool CreateObject(const BSFixedString& a_objectTypeName, BSTSmartPointer<Object>& a_newObj) override;                                                                                                                                                                                                    // 17
				virtual bool CreateObject(const BSFixedString& a_objectTypeName, const BSTScrapHashMap<BSFixedString, Variable>& a_properties, BSTSmartPointer<Object>& a_newObj) override;                                                                                                                                      // 16
				virtual bool CreateStruct(const BSFixedString& a_structTypeName, BSTSmartPointer<Struct>& a_newStruct) override;                                                                                                                                                                                                 // 18
				virtual bool CreateArray(TypeInfo::RawType a_elementType, const BSFixedString& a_elementObjectTypeName, std::uint32_t a_elementCount, BSTSmartPointer<Array>& a_newArray) override;                                                                                                                              // 1A
				virtual bool CreateArray(const TypeInfo& a_type, std::uint32_t a_elementCount, BSTSmartPointer<Array>& a_newArray) override;                                                                                                                                                                                     // 19
				virtual bool BindNativeMethod(IFunction* a_function) override;                                                                                                                                                                                                                                                   // 1B
				virtual void SetCallableFromTasklets(const char* a_objectName, const char* a_functionName, bool a_taskletCallable) override;                                                                                                                                                                                     // 1D
				virtual void SetCallableFromTasklets(const char* a_objectName, const char* a_stateName, const char* a_functionName, bool a_taskletCallable) override;                                                                                                                                                            // 1C
				virtual void ForEachBoundObject(std::uint64_t a_objHandle, const BSTThreadScrapFunction<BSContainer::ForEachResult(Object*)>& a_functor) override;                                                                                                                                                               // 1E
				virtual bool FindBoundObject(std::uint64_t a_objHandle, const char* a_objectTypeName, bool a_allowConst, BSTSmartPointer<Object>& a_attachedObj, bool a_exactMatch) const override;                                                                                                                              // 1F
				virtual void MoveBoundObjects(std::uint64_t a_sourceHandle, std::uint64_t a_destHandle) override;                                                                                                                                                                                                                // 20
				virtual void ResetAllBoundObjects(std::uint64_t a_objHandle) override;                                                                                                                                                                                                                                           // 21
				virtual bool CastObject(const BSTSmartPointer<Object>& a_sourceObj, const BSTSmartPointer<ObjectTypeInfo>& a_targetType, BSTSmartPointer<Object>& a_castedObj) override;                                                                                                                                         // 22
				virtual bool SetPropertyValue(const BSTSmartPointer<Object>& a_self, const char* a_propName, const Variable& a_newValue, const BSTSmartPointer<IStackCallbackFunctor>& a_callback) override;                                                                                                                     // 23
				virtual bool GetPropertyValue(const BSTSmartPointer<Object>& a_self, const char* a_propName, const BSTSmartPointer<IStackCallbackFunctor>& a_callback) override;                                                                                                                                                 // 24
				virtual bool GetVariableValue(std::uint64_t a_objHandle, const BSFixedString& a_scriptName, std::uint32_t a_varIndex, Variable& a_var) const override;                                                                                                                                                           // 26
				virtual bool GetVariableValue(const BSTSmartPointer<Object>& a_obj, std::uint32_t a_varIndex, Variable& a_var) const override;                                                                                                                                                                                   // 25
				virtual bool HandleImplementsEvent(std::uint64_t a_object, const BSFixedString& a_eventName) const override;                                                                                                                                                                                                     // 27
				virtual bool AddEventRelay(std::uint64_t a_sourceObject, const BSFixedString& a_eventName, const BSTSmartPointer<Object>& a_destObj) override;                                                                                                                                                                   // 28
				virtual void RemoveEventRelay(std::uint64_t a_sourceObject, const BSFixedString& a_eventName, const BSTSmartPointer<Object>& a_destObj) override;                                                                                                                                                                // 29
				virtual void RemoveAllEventRelays(const BSTSmartPointer<Object>& a_destObj) override;                                                                                                                                                                                                                            // 2A
				virtual void SendEvent(std::uint64_t a_objHandle, const BSFixedString& a_eventName, const BSTThreadScrapFunction<bool(BSScrapArray<Variable>&)>& a_arguments, const BSTThreadScrapFunction<bool(const BSTSmartPointer<Object>&)>& a_filter, const BSTSmartPointer<IStackCallbackFunctor>& a_callback) override;  // 2B
				virtual bool DispatchStaticCall(const BSFixedString& a_objName, const BSFixedString& a_funcName, const BSTThreadScrapFunction<bool(BSScrapArray<Variable>&)>& a_arguments, const BSTSmartPointer<IStackCallbackFunctor>& a_callback) override;                                                                   // 2C
				virtual bool DispatchMethodCall(std::uint64_t a_objHandle, const BSFixedString& a_objName, const BSFixedString& a_funcName, const BSTThreadScrapFunction<bool(BSScrapArray<Variable>&)>& a_arguments, const BSTSmartPointer<IStackCallbackFunctor>& a_callback) override;                                        // 2E
				virtual bool DispatchMethodCall(const BSTSmartPointer<Object>& a_self, const BSFixedString& a_funcName, const BSTThreadScrapFunction<bool(BSScrapArray<Variable>&)>& a_arguments, const BSTSmartPointer<IStackCallbackFunctor>& a_callback) override;                                                            // 2D
				virtual bool DispatchUnboundMethodCall(std::uint64_t a_objHandle, const BSTSmartPointer<BoundScript>& a_script, const BSFixedString& a_funcName, const BSTThreadScrapFunction<bool(BSScrapArray<Variable>&)>& a_arguments, const BSTSmartPointer<IStackCallbackFunctor>& a_callback) override;                   // 2F
				virtual bool IsWaitingOnLatent(std::uint32_t a_stackID) const override;                                                                                                                                                                                                                                          // 30
				virtual void ReturnFromLatent(std::uint32_t a_stackID, const Variable& a_retValue) override;                                                                                                                                                                                                                     // 31
				[[nodiscard]] virtual ErrorLogger& GetErrorLogger() const override;                                                                                                                                                                                                                                              // 32
				[[nodiscard]] virtual const IObjectHandlePolicy& GetObjectHandlePolicy() const override;                                                                                                                                                                                                                         // 34
				[[nodiscard]] virtual IObjectHandlePolicy& GetObjectHandlePolicy() override;                                                                                                                                                                                                                                     // 33
				[[nodiscard]] virtual const ObjectBindPolicy& GetObjectBindPolicy() const override;                                                                                                                                                                                                                              // 36
				[[nodiscard]] virtual ObjectBindPolicy& GetObjectBindPolicy() override;                                                                                                                                                                                                                                          // 35
				[[nodiscard]] virtual ISavePatcherInterface& GetSavePatcherInterface() override;                                                                                                                                                                                                                                 // 37
				virtual void RegisterForLogEvent(BSTEventSink<LogEvent>* a_sink) override;                                                                                                                                                                                                                                       // 38
				virtual void UnregisterForLogEvent(BSTEventSink<LogEvent>* a_sink) override;                                                                                                                                                                                                                                     // 39
				virtual void RegisterForStatsEvent(BSTEventSink<StatsEvent>* a_sink) override;                                                                                                                                                                                                                                   // 3A
				virtual void UnregisterForStatsEvent(BSTEventSink<StatsEvent>* a_sink) override;                                                                                                                                                                                                                                 // 3B
				virtual void PostCachedErrorToLogger(const ICachedErrorMessage& a_errorFunctor, ErrorLogger::Severity a_severity) const override;                                                                                                                                                                                // 3D
				virtual void PostCachedErrorToLogger(const ICachedErrorMessage& a_errorFunctor, std::uint32_t a_stackID, ErrorLogger::Severity a_severity) const override;                                                                                                                                                       // 3C
				// TODO: figure out if align 10h here fucks things up???

				// override (IVMObjectBindInterface) 14307D8D8
				virtual std::uint64_t GetBoundHandle(const BSTSmartPointer<Object>& a_objPtr) const override;                                                          // 01
				virtual void TypeCanBeBound(const BSFixedString& a_className, std::uint64_t a_handle) override;                                                        // 02
				virtual void BindObject(BSTSmartPointer<Object>& a_objPtr, std::uint64_t a_handle, bool a_conditional) override;                                       // 03
				virtual void HandleLoadedBinding(BSTSmartPointer<Object>& a_objPtr, std::uint64_t a_handle, bool a_conditional) override;                              // 04
				virtual void RemoveAllBoundObjects(std::uint64_t a_handle) override;                                                                                   // 05
				virtual void RemoveAllDiskLoadedBoundObjects(std::uint64_t a_handle) override;                                                                         // 06
				virtual void HandleCObjectDeletion(std::uint64_t a_handle) override;                                                                                   // 07
				virtual void UnbindObject(const BSTSmartPointer<Object>& a_objPtr) override;                                                                           // 08
				virtual bool CreateObjectWithProperties(const BSFixedString& a_className, std::uint32_t a_numProperties, BSTSmartPointer<Object>& a_objPtr) override;  // 09
				virtual bool InitObjectProperties(BSTSmartPointer<Object>& a_objPtr, void* a_property, bool a_arg3) override;                                          // 0A

				// TODO: CommonLibSSE doesn't have these, figure out why
				// override (IVMSaveLoadInterface) 14307D938
				virtual bool SaveGame(BSStorage& a_storage, BSScript::IHandleReaderWriter const& a_HandleReaderWriter, bool a_flag) override;                                                                   //14307D940 	// 01
				virtual bool LoadGame(BSStorage const& a_storage, BSScript::IHandleReaderWriter const& a_HandleReaderWriter, bool& a_flag, bool& b_flag) override;                                              //14307D948 	// 02
				virtual void MarkSaveInvalid(BSStorage& a_storage) override;                                                                                                                                    //14307D950 	// 03
				virtual unsigned short GetSaveGameVersion() const override;                                                                                                                                     //14307D958 	// 04
				virtual void CleanupSave() override;                                                                                                                                                            //14307D960 	// 05
				virtual void CleanupLoad() override;                                                                                                                                                            //14307D968 	// 06
				virtual void DropAllRunningData() override;                                                                                                                                                     //14307D970 	// 07
				virtual std::uint64_t GetSaveHandleForObject(const BSScript::Object* a_Object) const override;                                                                                                  //14307D978 	// 08
				virtual void SetSaveHandleForObject(const BSScript::Object* a_Object, std::uint64_t) override;                                                                                                  //14307D980 	// 09
				virtual bool GetObjectBySaveHandle(std::uint64_t, const BSScript::TypeInfo& a_TypeInfo, BSTSmartPointer<BSScript::Object, BSTSmartPointerIntrusiveRefCount>& a_object_pointer) const override;  //14307D988 	// 0A
				virtual bool GetObjectBySaveHandle(std::uint64_t, BSTSmartPointer<BSScript::Object, BSTSmartPointerIntrusiveRefCount>& a_object_pointer) const override;                                        //14307D990 	// 0B
				virtual std::uint64_t GetSaveHandleForStruct(const BSScript::Struct* a_Struct) const override;                                                                                                  //14307D998 	// 0C
				virtual void SetSaveHandleForStruct(const BSScript::Struct* a_Struct, std::uint64_t) override;                                                                                                  //14307D9A0 	// 0D
				virtual bool GetStructBySaveHandle(std::uint64_t, BSTSmartPointer<BSScript::Struct, BSTSmartPointerIntrusiveRefCount>& a_object_pointer) const override;                                        //14307D9A8 	// 0E
				virtual std::uint64_t GetSaveHandleForArray(const BSScript::Array* a_Array) const override;                                                                                                     //14307D9B0 	// 0F
				virtual void SetSaveHandleForArray(const BSScript::Array* a_Array, std::uint64_t) override;                                                                                                     //14307D9B8 	// 10
				virtual bool GetArrayBySaveHandle(std::uint64_t handle, BSTSmartPointer<BSScript::Array, BSTSmartPointerIntrusiveRefCount>& a_array_pointer) const override;                                    //14307D9C0 	// 11
				virtual bool GetStackByID(unsigned int, BSTSmartPointer<BSScript::Stack, BSTSmartPointerIntrusiveRefCount>& a_stack_pointer) const override;                                                    //14307D9C8 	// 12
				virtual const BSScript::Internal::WritableStringTable& GetWritableStringTable() const override;                                                                                                 //14307D9D0 	// 13
				virtual const BSScript::Internal::WritableStringTable& GetWritableStringTable() override;                                                                                                       //14307D9D8 	// 14
				virtual BSScript::Internal::ReadableStringTable& GetReadableStringTable() const override;                                                                                                       //14307D9E0 	// 15
				virtual const BSScript::Internal::WritableTypeTable& GetWritableTypeTable() const override;                                                                                                     //14307D9E8 	// 16
				virtual BSScript::Internal::WritableTypeTable& GetWritableTypeTable() override;                                                                                                                 //14307D9F0 	// 17
				virtual const BSScript::Internal::ReadableTypeTable& GetReadableTypeTable() const override;                                                                                                     //14307D9F8 	// 18
				virtual bool CreateEmptyTasklet(BSScript::Stack* a_Stack, BSTSmartPointer<BSScript::Internal::CodeTasklet, BSTSmartPointerIntrusiveRefCount>& a_tasklet_pointer) override;                      //14307DA00 	// 19
				// override (IVMDebugInterface)
				virtual void DumpRunningStacksToLog() override;                                                                                // 01   // 14307DA18
				virtual void DumpStackFrameToLog(unsigned int a_v, unsigned int b_v, bool a_flag) override;                                    // 02   // 14307DA20
				virtual void GetStackFrame(unsigned int a_v, unsigned int b_v, bool a_flag, BSFixedString& a_identifier) override;             // 03   // 14307DA28
				virtual void DumpPersistenceInformationToLog(char const* logfile, uint64_t a_v) const override;                                // 04   // 14307DA30
				virtual void DumpEventRelayInformationToLog(char const* logfile, uint64_t a_v, BSFixedString const& a_string) const override;  // 05   // 14307DA38

				static VirtualMachine* GetSingleton();

				//bool StackExists(std::uint32_t stack_id);

				// members
				ErrorLogger* errorLogger;             // 0080
				IMemoryPagePolicy* memoryPagePolicy;  // 0088
				IObjectHandlePolicy* handlePolicy;    // 0090
				ObjectBindPolicy* objectBindPolicy;   // 0098
				//IFreezeQuery*                                              freezeQuery;                 // 00A0
				IClientVM* clientVM;                                                       // 00A0
				IStackCallbackSaveInterface* stackCallbackSaveInterface;                   // 00A8
				IProfilePolicy* profilePolicy;                                             // 00B0
				ISavePatcherInterface* savePatcherInterface;                               // 00B8
				mutable BSSpinLock typeInfoLock;                                           // 00C0
				LinkerProcessor linker;                                                    // 00C8
				BSTHashMap<BSFixedString, BSTSmartPointer<ObjectTypeInfo>> objectTypeMap;  // 0168     0158
				BSTHashMap<BSFixedString, BSTSmartPointer<StructTypeInfo>> structTypeMap;  // 0198
				BSTHashMap<std::uint32_t, BSFixedString> typeIDToObjectType;               // 01C8     0188
				BSTHashMap<BSFixedString, std::uint32_t> objectTypeToTypeID;               // 01F8     01B8
				BSTArray<BSTSmartPointer<ObjectTypeInfo>> typesToUnload;                   // 0228     01E8
				mutable BSSpinLock funcQueueLock;                                          // 0240     0200
				BSTStaticFreeList<FunctionMessage, 1024> funcMsgPool;                      // 0248     size == 0x8018

				BSTCommonLLMessageQueue<FunctionMessage> funcMsgQueue;  // 8260 8220
				BSTArray<FunctionMessage> overflowFuncMsgs;             // 8288     8248
				BSTArray<CodeTasklet*> vmTasks;                         // 82A0     8260
				std::uint32_t uiWaitingFunctionMessages;                // 82B8     8278
				bool overstressed;                                      // 82BC     827C
				bool initialized;                                       // 82BD     827D
				char pad82BE[0x42];                                     // 82BE
				// TODO: what's between here and overflowsuspencd array
				char gap8300[0x3A00];
				// BSTCommonSTaticMessageQueue changed between Skyrim and Fallout4, it contains a statically allocated lockless queue now,
				// need to figure out how BSTLocklessQueue is structured
				// BSTCommonStaticMessageQueue<SuspendedStack, 128>           suspendQueue1;               // 8300 size??? vftable (0x80 + 80C)
				//	 A bunch of stuff here
				// start pointer 8330
				// BSTLocklessQueue::ObjQueueBase

				// BSTCommonStaticMessageQueue<SuspendedStack, 128>           suspendQueue2;               // A000
				// objqueue A080

				BSTArray<SuspendedStack> overflowSuspendArray1;                          // BD00
				BSTArray<SuspendedStack> overflowSuspendArray2;                          // BD18
				mutable BSSpinLock suspendQueueLock;                                     // BD30
				BSTCommonStaticMessageQueue<SuspendedStack, 128>* stacksToResume;        // BD38 - ref to suspendQueue2 @ 8300
				BSTArray<SuspendedStack>* stacksToResumeOverflow;                        // BD40 - ref to overflowSuspendArray2 @BD00
				BSTCommonStaticMessageQueue<SuspendedStack, 128>* stacksToSuspend;       // BD48 - ref to suspendQueue1 @A000
				BSTArray<SuspendedStack>* stacksToSuspendOverflow;                       // BD50 - ref to overflowSuspendArray1 @BD18
				mutable BSSpinLock runningStacksLock;                                    // BD58
				BSTHashMap<std::uint32_t, BSTSmartPointer<Stack>> allRunningStacks;      // BD60
				BSTHashMap<std::uint32_t, BSTSmartPointer<Stack>> waitingLatentReturns;  // BD90
				std::uint32_t nextStackID;                                               // BDC0
				mutable BSSpinLock frozenStacksLock;                                     // BDC4
				std::uint32_t pad938C;                                                   // BDCC
				mutable BSSpinLock someLockBDC8;                                         // BDC8
				BSTArray<void*> SomeArrayBD00;                                           // BDD0

				BSScript::Stack* frozenStacks;                             // BDE8
				std::uint32_t frozenStacksCount;                           // BDF0
				stl::enumeration<FreezeState, std::uint32_t> freezeState;  // BDF4

				mutable BSSpinLock attachedScriptsLock;                                     // BDF8
				BSTHashMap<uint64_t, BSTSmallSharedArray<AttachedScript>> attachedScripts;  // BE00
				std::uint32_t unknownBE30;                                                  // BE30
				std::uint32_t padBE34;                                                      // BE34
				std::uint64_t _somethingToDoWithIObjectHandlePolicy;                        // BE38
				// TODO: The rest of this
				char gapBE40[0x1FC];

				// BSTArray<BSTSmartPointer<Struct>>						   structsAwatingReset;     	// BE40
				// // Don't know where this is??
				// mutable BSSpinLock										   structResetLock;				// ???? BE58???

				//32 BE58
				//32 BE5C
				//32 BE60
				//pad32 BE64

				// mutable BSSpinLock                                         arraysLock;                  //          BE58
				// std::uint32_t                                              nextArrayToClean;            //          BE5C
				// std::uint32_t                                              padBE60;                     //          BE60
				// BSTArray<BSTSmartPointer<Array>>                           arrays;                      // BE68

				//ARRAY BE68
				//32 BE80
				//32 BE84
				//32 BE88
				//pad32 BE8C
				//ARRAY BE90
				//32 BEA8
				//32 BEAC
				//ARRAY BE90

				// // std::uint64_t                                              unk93D8;                  //          93D8
				// // BSTArray<void*>                                            unk93E0;                  //          93E0
				// mutable BSSpinLock                                         arraysLock;                  //          93F8
				// std::uint32_t                                              nextArrayToClean;            //          9400
				// std::uint32_t                                              pad9404;                     //          9404
				// BSTArray<BSTSmartPointer<Array>>                           arrays;                      // BE68
				// mutable BSSpinLock                                         objectResetLock;             //          9420
				// BSTArray<BSTSmartPointer<Object>>                          objectsAwaitingReset;        //          9428
				// std::uint64_t                                              unk9440;                     //          9440
				// BSTHashMap<UnkKey, UnkValue>                               unk9448;                     //          9448
				// BSTHashMap<UnkKey, UnkValue>                               unk9478;                     //          9478
				// mutable BSSpinLock                                         queuedUnbindLock;            //          94A8
				// BSTArray<QueuedUnbindRefs>                                 queuedUnbinds;               //          94B0
				// std::uint64_t                                              unk94C8;                     //          94C8
				// std::uint64_t                                              unk94D0;                     //          94D0
				// std::uint64_t                                              unk94D8;                     //          94D8
				// std::uint64_t                                              unk94E0;                     //          94E0
				// std::uint64_t                                              unk94E8;                     //          94E8
				// std::uint64_t                                              unk94F0;                     //          94F0
				// std::uint64_t                                              unk94F8;                     //          94F8
				// std::uint64_t                                              unk9500;                     //          9500
				// std::uint64_t                                              unk9508;                     //          9508
				// std::uint64_t                                              unk9518;                     //          9510
			};
			// Size == C03C
			// #pragma message("sizeof(VirtualMachine is ")
			// #pragma message(sizeof(VirtualMachine))
			//static_assert(sizeof(VirtualMachine) == C03C);
		}
	}
}
