package xxl.cells;

import java.io.Serializable;
import xxl.exceptions.InvalidResultException;
import xxl.cells.renderers.Renderer;
import xxl.cells.renderers.Renderable;

/**
 * Class representing a binary operation
 * 
 * @param <T> The type on which the binary operation is performed.
 */
public abstract class BinaryOperation<T> implements Serializable, Renderable {
	/** @param name of the operation */
	public BinaryOperation(String name) {
		_name = name;
	}

	/** @See xxl.cells.renderers.Renderable#render */
	public String render(Renderer renderer) {
		return renderer.renderBinaryOperation(this);
	}

	/** @return the name of this operation */
	public String getName() { return _name; }

	/**
	* @param operand1 The first operand.
	* @param operand2 The second operand.
	* @return The result of the binary operation.
	* @throws InvalidResultException if the operation cannot be
	* executed.
	*/
	abstract public T execute(T operand1, T operand2) throws InvalidResultException;

	private String _name;
}
