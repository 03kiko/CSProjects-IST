package xxl.cells;

import xxl.exceptions.InvalidResultException;
import xxl.cells.renderers.Renderer;
import xxl.cells.search.Search;
import xxl.storage.CellStorage;

/**
 * Class for representing cells that work with two integers.
 */
public class IntegerBinary extends Binary<Integer> {
	/**
	* @param operand1 first operand for the binary function.
	* @param operand2 second operand for the binary function.
	* @param operation The binary operation to be performed.
	*/
	public IntegerBinary(Content operand1, Content operand2,
				 BinaryOperation<Integer> operation) {
		super(operand1, operand2, operation);
	}

	/**
	* @return The result of the binary operation as an Integer.
	*/
	@Override
	public Integer getInteger() {
		try{
			Integer operand1 = getFirstOperand().getInteger();
			Integer operand2 = getSecondOperand().getInteger();
			if (operand1 == null || operand2 == null)
				return null;
			return getOperation().execute(operand1, operand2);
		} catch(InvalidResultException e) {
			return null;
		}
	}

	/** @See xxl.cells.renderers.Renderable#render */
	@Override
	public String render(Renderer renderer) {
		return renderer.renderIntegerBinary(this);
	}

	/** @See xxl.cells.search.Searchable#ok */
	@Override
	public boolean ok(Search search) {
		return search.okIntegerBinary(this);
	}

	/** @See Content#copyTo */
	@Override
	public Content copyTo(CellStorage dest, Position pos, boolean dependencies) {
		Content copiedOp1 = getFirstOperand().copyTo(dest, pos, dependencies);
		Content copiedOp2 = getSecondOperand().copyTo(dest, pos, dependencies);
		Content copied = new IntegerBinary(copiedOp1, copiedOp2, getOperation());
		dest.insert(pos, copied);
		return copied;
	}
}
