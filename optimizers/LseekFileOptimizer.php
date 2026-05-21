<?php

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

class LseekFileOptimizer extends OptimizerAbstract
{
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters']) || count($expression['parameters']) !== 3) {
            throw new CompilerException("'lseek_file' requires exactly three parameters", $expression);
        }

        $context->headersManager->add('src/system-api');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        return new CompiledExpression(
            'int',
            'posix_lseek('
            . $resolvedParams[0] . ', '
            . $resolvedParams[1] . ', '
            . $resolvedParams[2]
            . ')',
            $expression
        );
    }
}
