using System;

namespace CstGUI.CstCore
{
    public class CstComponent : CstNode, IDisposable
    {
        public CstComponent() : base()
        {
            Console.WriteLine("Mono CstComponent ok!");
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Dispose()
        {
            throw new NotImplementedException();
        }
    }
}
