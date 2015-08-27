#include "Editor/ExpressionAtom.h"
#include "Editor/Value.h"
#include "Network/Address.h"
#include "Network/Node.h"

using namespace OSSIA;
using namespace std;
using namespace std::placeholders;

class JamomaExpressionAtom : public ExpressionAtom
{
  
private:
  
# pragma mark -
# pragma mark Implementation specific
  
  Value*    mFirstValue;
  Operator  mOperator;
  Value*    mSecondValue;
  
  CallbackContainer<ValueCallback>::CallbackIterator mFirstValueCallback;
  CallbackContainer<ValueCallback>::CallbackIterator mSecondValueCallback;
  
public:
  
# pragma mark -
# pragma mark Life cycle
  
  JamomaExpressionAtom(const Value* value1, Operator op, const Value* value2) :
  mFirstValue(value1->clone()),
  mOperator(op),
  mSecondValue(value2->clone())
  {}
  
  JamomaExpressionAtom(const JamomaExpressionAtom * other)
  {}
  
  shared_ptr<ExpressionAtom> clone() const override
  {
    return make_shared<JamomaExpressionAtom>(this);
  }
  
  ~JamomaExpressionAtom()
  {}

# pragma mark -
# pragma mark Execution
  
  bool evaluate() const override
  {
    return do_evaluation(mFirstValue, mSecondValue);
  }
  
# pragma mark -
# pragma mark Callback Container
  
  CallbackContainer<ResultCallback>::CallbackIterator addCallback(ResultCallback callback) override
  {
    callbacks().push_back(callback);
    
    if (callbacks().size() == 1)
    {
      // start first operand observation if it is a Destination
      //! \todo what about Tuple of Destinations ?
      if (mFirstValue->getType() == Value::Type::DESTINATION)
      {
        Destination* d = (Destination*)mFirstValue;
        if (d->value->getAddress())
        {
          auto callback = std::bind(&JamomaExpressionAtom::firstValueCallback, this, _1);
          mFirstValueCallback = d->value->getAddress()->addCallback(callback);
        }
      }
      
      // start second operand observation if it is a Destination
      //! \todo what about Tuple of Destinations ?
      if (mSecondValue->getType() == Value::Type::DESTINATION)
      {
        Destination* d = (Destination*)mSecondValue;
        if (d->value->getAddress())
        {
          auto callback = std::bind(&JamomaExpressionAtom::secondValueCallback, this, _1);
          mSecondValueCallback = d->value->getAddress()->addCallback(callback);
        }
      }
    }
    
    return callbacks().end();
  }
  
  void removeCallback(CallbackContainer<ResultCallback>::CallbackIterator iterator) override
  {
    callbacks().erase(iterator);
    
    if (callbacks().size() == 0)
    {
      // stop first operand observation if it is a Destination
      //! \todo what about Tuple of Destinations ?
      if (mFirstValue->getType() == Value::Type::DESTINATION)
      {
        Destination* d = (Destination*)mFirstValue;
        if (d->value->getAddress())
        {
          d->value->getAddress()->removeCallback(mFirstValueCallback);
        }
      }
      
      // start second operand observation if it is a Destination
      //! \todo what about Tuple of Destinations ?
      if (mSecondValue->getType() == Value::Type::DESTINATION)
      {
        Destination* d = (Destination*)mSecondValue;
        if (d->value->getAddress())
        {
          d->value->getAddress()->removeCallback(mSecondValueCallback);
        }
      }
    }
  }

# pragma mark -
# pragma mark Accessors
  
  const Value* getFirstOperand() const override
  {
    return mFirstValue;
  }
  
  Operator getOperator() const override
  {
    return mOperator;
  }
  
  const Value* getSecondOperand() const override
  {
    return mSecondValue;
  }
  
# pragma mark -
# pragma mark Implementation Specific
  
  bool do_evaluation(const Value* first, const Value* second) const
  {
    switch (mOperator)
    {
      case Operator::EQUAL :
      {
        return (*first) == (*second);
      }
      case Operator::DIFFERENT :
      {
        return (*first) != (*second);
      }
      case Operator::GREATER_THAN :
      {
        return (*first) > (*second);
      }
      case Operator::LOWER_THAN :
      {
        return (*first) < (*second);
      }
      case Operator::GREATER_THAN_OR_EQUAL :
      {
        return (*first) >= (*second);
      }
      case Operator::LOWER_THAN_OR_EQUAL :
      {
        return (*first) <= (*second);
      }
      default :
        return false;
    }
  }
  
  void firstValueCallback(const Value * value)
  {
    bool result = do_evaluation(value, mSecondValue);
    
    for (auto callback : callbacks())
      callback(result);
  }
  
  void secondValueCallback(const Value * value)
  {
    bool result = do_evaluation(mFirstValue, value);
    
    for (auto callback : callbacks())
      callback(result);
  }

};

namespace OSSIA
{
  shared_ptr<ExpressionAtom> ExpressionAtom::create(const Value* value1, Operator op, const Value* value2)
  {
    return make_shared<JamomaExpressionAtom>(value1, op, value2);
  }
}