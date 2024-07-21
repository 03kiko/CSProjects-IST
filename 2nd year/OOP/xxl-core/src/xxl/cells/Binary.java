package xxl.cells;

import xxl.storage.CellStorage;

/**
 * Class for representing the content of cells that work on
 * two arguments
 * 
 * @param <T> The type of the binary function.
 */
public abstract class Binary<T> extends Content<Content> {
	/**
	* @param operand1 first operand for the binary function.
	* @param operand2 second operand for the binary function.
	* @param operation The binary operation to performed.
	*/
	public Binary(Content operand1, Content operand2,
			  BinaryOperation<T> operation) {
		super(operand1);
		_operand2 = operand2;
		_operation = operation;
	}

	protected Binary(Content op1, Content op2, BinaryOperation<T> operation, CellStorage dest) {
	}

	/**
	 * @return the first operand.
	 */
	public Content getFirstOperand() {
		return getContent();
	}

	/**
	* @return The second operand.
	*/
	public Content getSecondOperand() {
		return _operand2;
	}

	/**
	* @return The binary operation.
	*/
	public BinaryOperation<T> getOperation() {
		return _operation;
	}
	
	/** @See Content#initialize */
	@Override
	public void initialize(Cell cell) {
		getFirstOperand().initialize(cell);
		_operand2.initialize(cell);
	}

	/** @See Content#update */
	@Override
	public void update() {
		getFirstOperand().update();
		_operand2.update();
	}

	/** @See Content#destroy */
	@Override
	public void destroy(Cell cell) {
		getFirstOperand().destroy(cell);
		_operand2.destroy(cell);
	}

	/** Second operand. */
	private Content _operand2;

	/** Binary operation to be performed. */
	private BinaryOperation<T> _operation;
}
