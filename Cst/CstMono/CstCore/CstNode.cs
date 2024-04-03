using System;

namespace CstGUI.CstCore
{
    public class CstNode : IDisposable
    {
        public CstNode()
        {
            Console.WriteLine("Mono CstNode ok!");
        }

        public virtual void Dispose()
        {
            throw new NotImplementedException();
        }
    }
}
