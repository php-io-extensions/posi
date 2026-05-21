<?php

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

class GetuidProcOptimizer extends OptimizerAbstract
{
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (isset($expression['parameters']) && count($expression['parameters']) !== 0) {
            throw new CompilerException("'getuid_proc' requires zero parameters", $expression);
        }

        $context->headersManager->add('src/system-api');

        return new CompiledExpression('int', 'posix_getuid()', $expression);
    }
}
