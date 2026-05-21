<?php

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

class ReadvFileOptimizer extends OptimizerAbstract
{
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters']) || count($expression['parameters']) !== 2) {
            throw new CompilerException("'readv_file' requires exactly two parameters", $expression);
        }

        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariable($symbolVariable, $expression);

        $context->headersManager->add('kernel/array');
        $symbolVariable->setDynamicTypes(['array', 'bool']);

        $resolvedParams = $call->getReadOnlyResolvedParams(
            $expression['parameters'],
            $context,
            $expression
        );

        $this->checkInitSymbolVariable($call, $symbolVariable, $context);

        $symbol = $context->backend->getVariableCode($symbolVariable);

        $context->headersManager->add('src/system-api');

        $context->codePrinter->output(
            'posix_readv(' . $symbol . ', ' . $resolvedParams[0] . ', ' . $resolvedParams[1] . ');'
        );

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
