#include "Progress.h"

namespace fm {

static Progress* default_progress = NULL;

Progress* Progress::GetDefault()
{
	return default_progress;
}

void Progress::SetDefault(Progress* prog)
{
	default_progress = prog;
}

}
