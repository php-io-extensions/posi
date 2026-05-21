<?php

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

class FcntlFileOptimizer extends OptimizerAbstract
{
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters']) || count($expression['parameters']) !== 4) {
            throw new CompilerException("'fcntl_file' requires exactly four parameters", $expression);
        }

        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable();
        if (!$symbolVariable->isInt()) {
            throw new CompilerException("fcntl result must be stored in an int variable", $expression);
        }

        $context->headersManager->add('src/system-api');

        $resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);

        return new CompiledExpression(
            'int',
            'posix_fcntl('
            . $resolvedParams[0] . ', '
            . $resolvedParams[1] . ', '
            . $resolvedParams[2] . ', '
            . $resolvedParams[3]
            . ')',
            $expression
        );
    }
}
