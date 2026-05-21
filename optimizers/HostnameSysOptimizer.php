<?php

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

class HostnameSysOptimizer extends OptimizerAbstract
{
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (isset($expression['parameters']) && count($expression['parameters']) !== 0) {
            throw new CompilerException("'hostname_sys' requires zero parameters", $expression);
        }

        $call->processExpectedReturn($context);

        $symbolVariable = $call->getSymbolVariable(true, $context);
        $this->checkNotVariableString($symbolVariable, $expression);

        $context->headersManager->add('kernel/string');
        $symbolVariable->setDynamicTypes('string');

        $context->headersManager->add('src/system-api');

        $this->checkInitSymbolVariable($call, $symbolVariable, $context);

        $symbol = $context->backend->getVariableCode($symbolVariable);

        $context->codePrinter->output('posix_hostname(' . $symbol . ');');

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }
}
