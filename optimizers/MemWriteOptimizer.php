<?php

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

class MemWriteOptimizer extends OptimizerAbstract
{
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        $paramCount = isset($expression['parameters']) ? count($expression['parameters']) : 0;

        if ($paramCount < 2 || $paramCount > 3) {
            throw new CompilerException("'mem_write' requires 2 or 3 parameters", $expression);
        }

        $context->headersManager->add('src/memory-api');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        $context->codePrinter->output(
            'posi_mem_write(' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ', ' . $resolvedParams[2] . ');'
        );

        return new CompiledExpression('null', 'null', $expression);
    }
}
