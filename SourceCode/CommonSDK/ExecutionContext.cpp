#include "ExecutionContext.h"

namespace fm {

ExecutionContext::ExecutionContext() 
	: multi_threaded(true),
	  io_thread_allowed(true),
	  big_memory(false)
{
}

ExecutionContext::~ExecutionContext()
{
}

static ExecutionContext global_execution_context;

ExecutionContext& ExecutionContext::GetCurrent()
{
	return global_execution_context;
}

void ExecutionContext::UseDesktopContext(bool enable_gpus)
{
	ExecutionContext& ctx = global_execution_context;
	ctx.multi_threaded    = true;
	ctx.io_thread_allowed = true;
	ctx.big_memory        = false;

	delete ctx.rlimit_cpu.resource_id;
	ctx.rlimit_cpu.resource_id  = NULL;
	ctx.rlimit_cpu.limited_size = RESOURCE_UNLIMITED;

	delete ctx.rlimit_gpu.resource_id;
	ctx.rlimit_gpu.resource_id  = NULL;
	ctx.rlimit_cpu.limited_size = enable_gpus ? RESOURCE_UNLIMITED : 0;
}

static void CopyComputeResourceLimit(bool exclusive, ExecutionContext::ResourceLimit& dest, const ExecutionContext::ResourceLimit* src)
{
	delete dest.resource_id;
	dest.resource_id = NULL;
	if (!src)
		dest.limited_size = exclusive ? RESOURCE_UNLIMITED : 1;
	else {
		dest.limited_size = src->limited_size;
		if (src->limited_size > 0) {
			dest.resource_id = new int[src->limited_size];
			memcpy(dest.resource_id, src->resource_id, sizeof(int)*src->limited_size);
		}
	}
}

void ExecutionContext::UseComputeContext(bool exclusive, const ResourceLimit* rcpu, 
	const ResourceLimit* rgpu, const ResourceLimit* rmem)
{
	ExecutionContext& ctx = global_execution_context;
	ctx.multi_threaded    = exclusive;
	ctx.io_thread_allowed = true;
	ctx.big_memory        = true;

	CopyComputeResourceLimit(exclusive, ctx.rlimit_cpu, rcpu);
	CopyComputeResourceLimit(exclusive, ctx.rlimit_gpu, rgpu);
	CopyComputeResourceLimit(exclusive, ctx.rlimit_mem, rmem);
}

ExecutionContext::ResourceLimit::ResourceLimit()
	: limited_size(RESOURCE_UNLIMITED),
	  resource_id(NULL)
{
}

ExecutionContext::ResourceLimit::~ResourceLimit()
{
	if (resource_id)
		delete[] resource_id;
	resource_id = NULL;
}

}
